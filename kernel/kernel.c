/*
 * File:		kernel.c
 * Desciption:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */
#include "shell/font.h"

void _start(struct UefiKernelInterface* interface){
	struct FrameBuffer* frame_buffer = interface->frame_buffer_ptr;
	unsigned int y = 300;
	unsigned int bbp = 4;
	for(unsigned int x  = 0; x < (frame_buffer->width / 2) * bbp; x+=bbp){
		unsigned int* pixel_ptr = frame_buffer->base_ptr + (frame_buffer->ppsl * bbp * y) + x;
		*pixel_ptr = 0x123456ff;
	}
}