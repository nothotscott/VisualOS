/*
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with VisualOS for licensing terms
 */

#include <stdio.h>
#include "pdclib/_PDCLIB_glue.h"

typedef long ssize_t;

ssize_t write( int, const void *, size_t );


ssize_t write( int fd, const void * buf, size_t count ) {
	return 0;
}