/*
 * File:		syscall.h
 * Description:	Configure userspace and kernel mode
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once


// Enable system call extensions and sets the STAR
void syscall_enable_sce();

// Enters [code] with [stack] in usermode
void syscall_goto_userspace(void* code, void* stack);