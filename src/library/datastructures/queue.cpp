/*
 * File:		queue.cpp
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "node.h"
#include "queue.h"

using namespace libvos;


template<typename T>
Queue<T>::Queue() : m_head { NULL } {
}

template<typename T>
Queue<T>::~Queue() {
}


template<typename T>
void Queue<T>::enqueue(T value) {
	DoubleNode<T>* node = new DoubleNode<T>();
	node->value = value;
	if(m_head == NULL) {
		m_head = node;
		return;
	}
	DoubleNode<T>* last = m_head;
	while(last->next != NULL){
		last = last->next;
	}
	last->next = node;
}

template<typename T>
void Queue<T>::dequeue(T& value) {
	value = m_head->value;
	if(m_head->next == NULL){
		delete m_head;
		m_head = NULL;
		return;
	}
	DoubleNode<T>* new_head = m_head->next;
	delete m_head;
	m_head = new_head;
}


extern "C" {

}