/*
 * File:		queue.h
 * Description:	Queue data structure
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

// TODO thread saftey?
#ifdef __cplusplus
namespace libvos {

template<typename T>
class Queue {
	private:
		DoubleNode<T>	m_head;
	public:
		Queue();
		~Queue();
		// Enqueues [value] into the queue
		void enqueue(T value);
		// Dequeues the head of the queue into [out]
		void dequeue(T& out);
		// Moves the head of the queue into [out] without removing it from the queue
		void peek(T& out);
		// Returns true if the queue is empty
		bool is_empty();
};


}

#else
struct Queue;

struct Queue* queue_init();
void queue_enqueue(struct Queue*);
void queue_dequeue(struct Queue*);

#endif