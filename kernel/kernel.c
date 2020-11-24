#include <bootloader.h>
#include <kernel.h>

#include "shell/font.h"

void _start(struct FrameBuffer* frame_buffer){
	unsigned int y = test();
	unsigned int bbp = 4;
	for(unsigned int x  = 0; x < (frame_buffer->width / 2) * bbp; x+=bbp){
		unsigned int* pixel_ptr = frame_buffer->base_ptr + (frame_buffer->ppsl * bbp * y) + x;
		*pixel_ptr = 0x123456ff;
	}
}