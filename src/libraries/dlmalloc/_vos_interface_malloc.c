/*
 * File:		_vos_interface_malloc.c
 * Description:	Provides an interface between the VOS abstraction layers
 * 				and the malloc library
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "_vos_syscall.h"
#include "syscall.h"


void* mmap(void* addr, size_t len, int prot, int flags, int fd, size_t offset) {
	return (void*)_VOS_SYSCALL(SYS_mmap, addr, len, prot, flags, fd, offset);
}

void* munmap(void* addr, size_t len){
	return (void*)_VOS_SYSCALL(SYS_munmap, addr, len);
}