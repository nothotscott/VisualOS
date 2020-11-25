/*
 * File:		kernel.c
 * Desciption:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "shell/shell.h"

void _start(struct UefiKernelInterface* interface){
	struct FrameBuffer* frame_buffer = interface->frame_buffer_ptr;
	struct PSF1Font* font = interface->font_ptr;

	for(uint i = 0; i < 50; i++){
		print(frame_buffer, font, "Hello, kernel!", 0xffffffff);
	}
}

