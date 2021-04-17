/*
 * File:		files.h
 * Description:	VOS-specific file defs
 * *****************************************************************************
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Code is partially from syscall_arch.h from musl
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "pdclib/_PDCLIB_internal.h"

#define VOS_FILE_FLAG_READ		_PDCLIB_FREAD
#define VOS_FILE_FLAG_WRITE		_PDCLIB_FWRITE
#define VOS_FILE_FLAG_APPEND	_PDCLIB_FAPPEND
#define VOS_FILE_FLAG_RW		_PDCLIB_FRW
#define VOS_FILE_FLAG_BIN		_PDCLIB_FBIN