/*
 * File:		scheduler.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "log.h"
#include "x86_64/gdt.h"
#include "scheduler.h"

#define DEFAULT_RPL			3
#define DEFAULT_STACK_PAGES	4

struct SchedulerQueue {
	struct SchedulerNode*	first;
	size_t					num;
};
// Enqueues task [node] in [queue]
static inline void _scheduler_enqueue_task(struct SchedulerNode* node, struct SchedulerQueue* queue);

static const uint64_t pass_flags = (1 << SCHEDULER_CONTEXT_FLAG_LOCKED) | (1 << SCHEDULER_CONTEXT_FLAG_FINISHED);
static struct SchedulerNode s_idle_node = {
	.context.context_frame.rip = (uint64_t)scheduler_idle,
	.context.context_frame.cs = GDT_OFFSET_KERNEL_CODE,
	.context.context_frame.rflags = RFLAGS,
	.context.context_frame.ss = GDT_OFFSET_KERNEL_DATA,
	.previous = &s_idle_node,
	.next = &s_idle_node
};
static struct SchedulerQueue s_queues[SCHEDULER_QUEUES_NUM];


// HACK this is temporary
// TODO use real malloc
static void* malloc(size_t size) {
	return pageframe_request_pages(NEAREST_PAGE(size));
}

static inline void _scheduler_enqueue_task(struct SchedulerNode* node, struct SchedulerQueue* queue) {
	struct SchedulerNode* first = queue->first;
	if(queue->num == 0) {
		node->previous = node;
		node->next = node;
		queue->first = node;
	} else {
		node->previous = first->previous;
		node->next = first;
		first->previous->next = node;
		first->previous = node;
	}
	queue->num++;
}
// TODO thread safety
struct SchedulerNode* scheduler_add_task(struct SchedulerTaskInitialState* initial_state, enum SchedulerQueueNumber queue_num) {
	struct SchedulerNode* node = malloc(sizeof(struct SchedulerNode));
	*node = (struct SchedulerNode){
		.queue_num = queue_num,
		.context.context_frame.general = initial_state->general,
		.context.context_frame.rip = (uint64_t)initial_state->entry,
		.context.context_frame.cs = (initial_state->rpl == 3 ? GDT_OFFSET_USER_CODE : GDT_OFFSET_KERNEL_CODE) | initial_state->rpl,
		.context.context_frame.rflags = RFLAGS,
		.context.context_frame.rsp = (uint64_t)initial_state->stack,
		.context.context_frame.ss = (initial_state->rpl == 3 ? GDT_OFFSET_USER_DATA : GDT_OFFSET_KERNEL_DATA) | initial_state->rpl
	};
	_scheduler_enqueue_task(node, s_queues + queue_num);
	return node;
}
struct SchedulerNode* scheduler_add_task_default(void* entry, size_t code_pages, enum SchedulerQueueNumber queue_num) {
	size_t stack_size = DEFAULT_STACK_PAGES * MEMORY_PAGE_SIZE;
	void* stack = pageframe_request_pages(DEFAULT_STACK_PAGES);
	paging_identity_map(stack, stack_size);
	for(size_t i = 0; i < DEFAULT_STACK_PAGES; i++){
		paging_set_userspace_access(stack + (i * MEMORY_PAGE_SIZE), true);
	}
	paging_set_userspace_access(stack, true);
	for(size_t i = 0; i < code_pages; i++){
		paging_set_userspace_access(entry + (i * MEMORY_PAGE_SIZE), true);
	}
	struct SchedulerTaskInitialState default_state = (struct SchedulerTaskInitialState){
		.entry = entry,
		.stack = stack + stack_size,
		.rpl = DEFAULT_RPL
	};
	return scheduler_add_task(&default_state, queue_num);
}

// TODO thread safety
__attribute__((hot)) struct SchedulerNode* scheduler_next_task(struct SchedulerNode* current) {
	struct SchedulerNode* next = NULL;
	if(current != 0) {
		if(current->context.flags & (1 << SCHEDULER_CONTEXT_FLAG_FINISHED)) {
			log_default("finished\n");
			// TODO free node
		} else {
			current->context.flags &= ~(1 << SCHEDULER_CONTEXT_FLAG_LOCKED);	// unlock
			s_queues[current->queue_num].first = current->next;					// rotate queue
		}
	}
	for(size_t i = 0; i < SCHEDULER_QUEUES_NUM; i++) {
		struct SchedulerQueue* queue = s_queues + i;
		struct SchedulerNode* node = queue->first;
		if(node == NULL) {
			continue;
		}
		for(size_t j = 0; j < queue->num; j++) {
			if(!(node->context.flags & pass_flags)) {
				next = node;
				goto exit;
			}
			node = node->next;
		}
	}
	if(next != NULL) {
		next->context.flags |= 1 << SCHEDULER_CONTEXT_FLAG_LOCKED;
	} else {
		next = &s_idle_node;
	}
	exit:
		return next;
}