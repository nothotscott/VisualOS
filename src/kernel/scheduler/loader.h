/*
 * File:		loader.h
 * Description:	Unknown
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


#define LOADER_DEFAULT_STACK_PAGES	3


// Begins execution of the elf file [module] as a process
void loader_execute_elf(void* module);