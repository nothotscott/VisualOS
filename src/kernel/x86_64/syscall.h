/*
 * File:		syscall.h
 * Description:	Configure userspace and kernel mode
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

// Enable system call extensions and sets the STAR
void syscall_enable();

// Enters [code] with [stack] in usermode
// This function is deprecated, use the scheduler instead
void syscall_goto_userspace(void* code, void* stack) __attribute__((deprecated));