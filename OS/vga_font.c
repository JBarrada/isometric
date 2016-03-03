#include <gfx.h>
#include <vga_font.h>
#include <string.h>
#include <smallfont.h>
#include <stdint.h>

int putch(int x, int y, char c, unsigned char color) {
	uint8_t char_width = smallfont[(uint8_t)c][0];
	uint8_t char_height = 8;
	
	uint8_t ix, iy, bpr;
	uint8_t bit;
	
	bpr = (char_width/8);
	bpr += (char_width%8)?1:0;
	
	for (iy=0; iy<char_height; iy++) {
		for (ix=0; ix<char_width; ix++) {
			bit = smallfont[(uint8_t)c][1 + (bpr*(8-iy-1)) + (ix/8)] >> (ix%8);
			bit &= 1;
			if (bit) {
				set_pixel(x+ix, y+iy, color);
			}
		}
	}
	
	return char_width;
}

void putstr(int x, int y, char *data, unsigned char color) {
	int datalen = strlen(data);
	
	int char_width = 0;
	for (int i = 0; i < datalen; i++) {
		char_width = putch(x, y, data[i], color);
		x += char_width+1;
	}
}