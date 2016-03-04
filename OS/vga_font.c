#include <gfx.h>
#include <vga_font.h>
#include <string.h>
#include <stdint.h>

TCLISTP *fonts[3] = {smallfont, axis, axis_big};
uint8_t heights[3] = {8, 16, 25};
uint8_t spacings[3] = {1, 2, 3};

int putch(int x, int y, char c, uint8_t color, uint8_t font) {
	uint8_t char_width = fonts[font][(uint8_t)c][0];
	uint8_t char_height = heights[font];
	
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

void putstr(int x, int y, char *data, uint8_t color, uint8_t font) {
	int datalen = strlen(data);
	
	int char_width = 0;
	for (int i = 0; i < datalen; i++) {
		char_width = putch(x, y, data[i], color, font);
		x += char_width+spacings[font];
	}
}