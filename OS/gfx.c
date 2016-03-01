#include <stdint.h>

#include <vga.h>
#include <io.h>
#include <gfx.h>

#define SW 320
#define SH 200

uint8_t back_buffer[320*200];
uint8_t change_buffer[25];

void row_change(uint8_t y) {
	change_buffer[y/8] |= (1<<(y%8));
}

void row_reset(uint8_t y) {
	change_buffer[y/8] &= ~(1<<(y%8));
}

void set_pixel(int x, int y, uint8_t c) {
	if ((x<SW) && (y<SH) && (x>=0) && (y>=0)) {
		if (back_buffer[(SH-y-1)*SW+x] != c) {
			back_buffer[(SH-y-1)*SW+x] = c;
			row_change(SH-y-1);
		}
	}
}

void circle_filled(int cx, int cy, int r, uint8_t c) {
	int i, tx, ty;
	int r2 = r * r;
	int area = r2 << 2;
	int rr = r << 1;

	for (i=0; i<area; i++) {
		tx = (i % rr) - r;
		ty = (i / rr) - r;

		if (tx * tx + ty * ty <= r2)
			set_pixel(cx+tx, cy+ty, c);
	}
}

void clear() {
	memset(back_buffer, 254, 320*200);
	memset(change_buffer, 0xff, 25);
}

void flush() {
	for (uint8_t row=0; row<200; row++) {
		if ((change_buffer[row/8]>>(row%8))&1 == 1) {
			swap_line(&back_buffer[row*320], row);
			row_reset(row);
		}
	}
}

