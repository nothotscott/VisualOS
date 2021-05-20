/*
 * File:		scheduler.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "x86_64/atomic.h"
#include "x86_64/gdt.h"
#include "timer.h"
#include "scheduler.h"

#define DEFAULT_RPL			3
#define DEFAULT_STACK_PAGES	4

#define PAUSE()	__asm__ __volatile__ ("pause" : : : "memory")


struct SchedulerQueue {
	struct SchedulerNode*	first;
	size_t					num;
	uint64_t				flags;
};
// Enqueues/dequeues task [node] into the queue num described by it. Assumes a lock for [node] AND its queue has already been acquired
static inline void _scheduler_enqueue_task(struct SchedulerNode* node);
static inline void _scheduler_dequeue_task(struct SchedulerNode* node);
// Locks/unlocks the [node]'s context using the SCHEDULER_CONTEXT_FLAG_LOCKED bit as the mutex. Returns true on successful lock/unlock
static inline bool _scheduler_node_lock(struct SchedulerNode* node);
static inline bool _scheduler_node_unlock(struct SchedulerNode* node);
// Locks/unlocks the [queue] using the SCHEDULER_QUEUE_FLAG_LOCKED bit as the mutex. Returns true on successful lock/unlock
static inline bool _scheduler_queue_lock(struct SchedulerQueue* queue);
static inline bool _scheduler_queue_unlock(struct SchedulerQueue* queue);
// Tries to lock [queue] and will enter a spin lock until acquired.
static inline void _scheduler_queue_spinlock(struct SchedulerQueue* queue);


static const uint64_t pass_flags = (1 << SCHEDULER_CONTEXT_FLAG_LOCKED) | (1 << SCHEDULER_CONTEXT_FLAG_FINISHED);
static struct SchedulerNode s_idle_node = {
	.context.task.tid = 0,
	.context.context_frame.rip = (uint64_t)scheduler_idle,
	.context.context_frame.cs = GDT_OFFSET_KERNEL_CODE,
	.context.context_frame.rflags = RFLAGS,
	.context.context_frame.ss = GDT_OFFSET_KERNEL_DATA,
	.previous = &s_idle_node,
	.next = &s_idle_node
};
static timer_ticks_t s_ticks_table[SCHEDULER_QUEUES_NUM] = {
	[SCHEDULER_QUEUE_PRIORITY]	= 0x100000,
	[SCHEDULER_QUEUE_REGULAR]	= 0x200000,
	[SCHEDULER_QUEUE_BATCH]		= 0x400000,
};
static uint64_t s_tid_accumulator = 1;
static uint64_t s_pid_accumulator = 1;
static struct SchedulerQueue s_queues[SCHEDULER_QUEUES_NUM];


// *** Internal Functions *** //

static inline bool _scheduler_node_lock(struct SchedulerNode* node) {
	return atomic_lock(&node->context.flags, SCHEDULER_CONTEXT_FLAG_LOCKED);
}
static inline bool _scheduler_node_unlock(struct SchedulerNode* node) {
	return atomic_unlock(&node->context.flags, SCHEDULER_CONTEXT_FLAG_LOCKED);
}
static inline bool _scheduler_queue_lock(struct SchedulerQueue* queue) {
	return atomic_lock(&queue->flags, SCHEDULER_QUEUE_FLAG_LOCKED);
}
static inline bool _scheduler_queue_unlock(struct SchedulerQueue* queue) {
	return atomic_unlock(&queue->flags, SCHEDULER_QUEUE_FLAG_LOCKED);
}
static inline void _scheduler_queue_spinlock(struct SchedulerQueue* queue) {
	while(!_scheduler_queue_lock(queue)) {
		PAUSE();
		log_default("queue lock\n");
	}
}

static inline void _scheduler_enqueue_task(struct SchedulerNode* node) {
	struct SchedulerQueue* queue = s_queues + node->queue_num;
	if(queue->num == 0) {
		node->previous = node;
		node->next = node;
		queue->first = node;
	} else {
		struct SchedulerNode* first = queue->first;
		node->previous = first->previous;
		node->next = first;
		first->previous->next = node;
		first->previous = node;
	}
	queue->num++;
}
static inline void _scheduler_dequeue_task(struct SchedulerNode* node) {
	struct SchedulerQueue* queue = s_queues + node->queue_num;
	if(queue->num <= 1) {
		queue->first = NULL;
	} else {
		node->next->previous = node->previous;
		node->previous->next = node->next;
	}
	queue->num--;
}

// *** Regular Functions *** //

void scheduler_init() {
	memset(s_queues, 0, sizeof(s_queues));
}

struct SchedulerNode* scheduler_add_task(struct SchedulerTaskInitialState* initial_state, enum SchedulerQueueNumber queue_num) {
	struct SchedulerNode* node = malloc(sizeof(struct SchedulerNode));
	struct SchedulerQueue* queue = s_queues + queue_num;
	uint64_t tid = atomic_accumulate(&s_tid_accumulator, 1);
	log_default("tid: %d\n", tid);
	*node = (struct SchedulerNode){
		.queue_num = queue_num,
		.context.task.tid = tid, // TODO real thread ID
		.context.context_frame.general = initial_state->general,
		.context.context_frame.rip = (uint64_t)initial_state->entry,
		.context.context_frame.cs = (initial_state->rpl == 3 ? GDT_OFFSET_USER_CODE : GDT_OFFSET_KERNEL_CODE) | initial_state->rpl,
		.context.context_frame.rflags = RFLAGS,
		.context.context_frame.rsp = (uint64_t)initial_state->stack,
		.context.context_frame.ss = (initial_state->rpl == 3 ? GDT_OFFSET_USER_DATA : GDT_OFFSET_KERNEL_DATA) | initial_state->rpl
	};
	_scheduler_queue_spinlock(queue);
	_scheduler_enqueue_task(node);
	_scheduler_queue_unlock(queue);
	return node;
}
// TODO replace with something better somewhere else
struct SchedulerNode* scheduler_add_task_default(void* entry, size_t code_pages, enum SchedulerQueueNumber queue_num) {
	size_t stack_size = DEFAULT_STACK_PAGES * MEMORY_PAGE_SIZE;
	void* stack = pageframe_request_pages(DEFAULT_STACK_PAGES);
	paging_set_attribute(paging_get_pagetable_l4(), stack, DEFAULT_STACK_PAGES, PAGE_DIRECTORY_USERSPACE, true);
	paging_set_attribute(paging_get_pagetable_l4(), entry, code_pages, PAGE_DIRECTORY_USERSPACE, true);
	struct SchedulerTaskInitialState default_state = (struct SchedulerTaskInitialState){
		.entry = entry,
		.stack = stack + stack_size,
		.rpl = DEFAULT_RPL
	};
	return scheduler_add_task(&default_state, queue_num);
}

void scheduler_free_task(struct SchedulerNode* node) {
	if(node == 0 || node == &s_idle_node){
		return;
	}
	_scheduler_dequeue_task(node);
	free(node);
}

#include "x86_64/cpuid.h" // TODO remove

__attribute__((hot)) struct SchedulerNode* scheduler_next_task(struct SchedulerNode* current) {
	uint8_t lapic = cpuid_get_local_apic_id();
	struct SchedulerNode* next = NULL;
	size_t current_queue_num = -1;
	// Mutate current
	if(current != 0 && current != &s_idle_node) {
		if(!(current->context.flags & (1 << SCHEDULER_CONTEXT_FLAG_LOCKED))) {
			log_options((struct LogOptions){ LOG_TYPE_WARNING }, "Current scheduling context does not have a lock!\n");
			_scheduler_node_lock(current);
		}
		// Rotate queue and perform necessary operations
		current_queue_num = current->queue_num;
		struct SchedulerQueue* queue = s_queues + current_queue_num;
		_scheduler_queue_spinlock(queue);
		queue->first = current->next;	// rotate queue
		if(current->context.flags & (1 << SCHEDULER_CONTEXT_FLAG_FINISHED)) {
			log_default("finished\n");
			scheduler_free_task(current);
		} else {
			_scheduler_node_unlock(current);
		}
		_scheduler_queue_unlock(queue);
	}
	// Find next
	for(size_t i = 0; i < SCHEDULER_QUEUES_NUM; i++) {
		struct SchedulerQueue* queue = s_queues + i;
		struct SchedulerNode* node = queue->first;
		if(node == NULL) {
			continue;
		}
		if(queue->flags & SCHEDULER_CONTEXT_FLAG_LOCKED) {
			continue;
		}
		for(size_t j = 0; j < queue->num; j++) {
			if(!(node->context.flags & pass_flags)) {
				if(_scheduler_node_lock(node)){
					if(current_queue_num != j) {
						timer_set_ticks(s_ticks_table[j]);
					}
					//log_default("%d: grabbed %d\n", lapic, node->context.task.tid);
					next = node;
					goto exit;
				}
			}
			node = node->next;
		}
	}
	exit: {
		if(next == NULL) {
			next = &s_idle_node;
			//log_default("%d: dead\n", lapic);
		}
		return next;
	}
}