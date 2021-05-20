/*
 * File:		sys_memory.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */


#include <sys/mman.h>
#include "memory/memory.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "sys.h"

uint64_t sys_mmap(void *addr, size_t len, int prot, int flags, int fd, size_t offset) {
	struct PageTable* pagetable_l4 = paging_get_pagetable_l4();	// TODO get a process's L4 Pagetable
	size_t pages = NEAREST_PAGE(len);
	void* block = pageframe_request_pages(pages);
	if(prot | PROT_WRITE) {
		paging_set_attribute(pagetable_l4, block, pages, PAGE_DIRECTORY_WRITABLE, true);
	}
	return (uint64_t)block;
}

uint64_t sys_munmap(void* addr, size_t len) {
	size_t pages = NEAREST_PAGE(len);
	pageframe_free(addr, pages);
	return 0;
}