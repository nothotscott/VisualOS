/*
 * File:		syscall.h
 * Description:	Configure userspace and kernel mode
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


// Enable system call extensions and sets the STAR
void syscall_enable_sce();

// Enters [code] with [stack] in usermode
void syscall_goto_userspace(void* code, void* stack);