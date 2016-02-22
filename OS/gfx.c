#include <stdint.h>

#include <vga.h>
#include <io.h>
#include <gfx.h>

#define SW 320
#define SH 200

uint8_t back_buffer[320*200];

void set_pixel(uint16_t x, uint16_t y, uint8_t c) {
	if ((x<SW) && (y<SH) && (x>=0) && (y>=0)) {
		back_buffer[y*SW+x] = c;
	}	
}

void clear() {
	
}

void flush() {
	swap_buffers(&back_buffer);
}

