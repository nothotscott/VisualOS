/*
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with VisualOS for licensing terms
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include "pdclib/_PDCLIB_glue.h"


#define	TEXT_COLOR_WHITE	0xffffffff

typedef uint32_t	text_color_t;
typedef	long		ssize_t;

// Write out according to the file descriptor [fd], the [buf] of [count]
extern ssize_t write( int fd, const void * buf, size_t count );