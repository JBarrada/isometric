#include <gfx.h>
#include <vga_font.h>
#include <string.h>
#include <smallfont.h>

int putch(int x, int y, char c, unsigned char color) {
	int char_width = smallfont[c][0];
	int char_height = 8;
	
	int ix, iy, bpr;
	unsigned char bit;
	
	bpr = (char_width/8);
	bpr += (char_width%8)?1:0;
	
	for (iy=0; iy<char_height; iy++) {
		for (ix=0; ix<char_width; ix++) {
			bit = smallfont[c][1 + (bpr*(8-iy-1)) + (ix/8)] >> (ix%8);
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