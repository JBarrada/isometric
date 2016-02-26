#include <stdint.h>

#include <vga.h>
#include <io.h>
#include <gfx.h>

#define SW 320
#define SH 200


void set_pixel(int x, int y, uint8_t c) {
	if ((x<SW) && (y<SH) && (x>=0) && (y>=0)) {
		back_buffer[(SH-y)*SW+x] = c;
	}
}

void clear() {
	memset(back_buffer, 254, 320*200);
	//swap_buffers(back_buffer);
}

void flush() {
	swap_buffers(back_buffer);
}

