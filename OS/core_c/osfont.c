#include <gfx.h>
#include <osfont.h>
#include <string.h>
#include <stdint.h>

#include <string.h>
#include <debug.h>

OSFONT AXIS_S;

void init_osfont() {
	load_osfont(&_binary_game_FONTS_axis_s_osfont_start, &AXIS_S);
}

uint8_t load_osfont(uint8_t *osfont_data, OSFONT *osfont) {
	osfont->char_height = osfont_data[0];
	osfont->char_spacing = osfont_data[1];
	osfont->char_widths = &osfont_data[2];
	osfont->data = &osfont_data[258];
	
	uint16_t dw = 0;
	for (int ox=0; ox<256; ox++)
		dw += osfont->char_widths[ox];
	osfont->data_width = dw;
	
	return 1;
}

uint8_t putch(int x, int y, char c, OSFONT *osfont, uint8_t color) {
	uint16_t xo = 0;
	for (int ox=0; ox<c; ox++)
		xo += osfont->char_widths[(uint8_t)ox];
	
	for (int iy=0; iy<osfont->char_height; iy++) {
		for (int ix=0; ix<osfont->char_widths[(uint8_t)c]; ix++) {
			uint32_t f_offset = iy*osfont->data_width+ix+xo;
			if (osfont->data[f_offset] != 0)
				set_pixel(x+ix, y+iy, color);
		}
	}
	
	return osfont->char_widths[(uint8_t)c];
}

int fontstrlen(char *data, OSFONT *osfont) {
	int datalen = strlen(data);
	int strlen = (datalen-1)*osfont->char_spacing;
	for (int i=0; i<datalen; i++) {
		strlen += osfont->char_widths[(uint8_t)data[i]];
	}
	return strlen;
}

void putstr(int x, int y, char *data, OSFONT *osfont, uint8_t color) {
	int datalen = strlen(data);
	
	int char_width = 0;
	for (int i=0; i<datalen; i++) {
		char_width = putch(x, y, data[i], osfont, color);
		x += char_width+osfont->char_spacing;
	}
}