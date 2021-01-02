/*
 * File:		node.h
 * Description:	Variation of different nodes
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once


// Single node
#ifdef __cplusplus
template<typename T>
struct Node {
	Node*	next;
	T		value;
};
#else
struct DoubleNode;
#endif

// Doubly linked node
#ifdef __cplusplus
template<typename T>
struct DoubleNode {
	DoubleNode*	next;
	DoubleNode*	prev;
	T			value;
};
#else
struct DoubleNode;
#endif
