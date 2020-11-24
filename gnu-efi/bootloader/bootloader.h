#pragma once

struct FrameBuffer {
	void* base_ptr;
	unsigned long long size;	// size of buffer
	unsigned int width;
	unsigned int height;
	unsigned int ppsl;			// pixels per scanline
};