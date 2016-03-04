#ifndef __OSFONT_H_
#define __OSFONT_H_

#include <stdint.h>

typedef struct tagOSFONT {
	uint8_t char_height;
	uint8_t char_spacing;
	uint16_t data_width;
	uint8_t *char_widths;
	uint8_t *data;
} OSFONT;


extern uint8_t _binary_game_FONTS_axis_s_osfont_start;


extern OSFONT AXIS_S;

void init_osfont();
uint8_t load_osfont(uint8_t *osfont_data, OSFONT *osfont);
uint8_t putch(int x, int y, char c, OSFONT *osfont, uint8_t color);
int fontstrlen(char *data, OSFONT *osfont);
void putstr(int x, int y, char *data, OSFONT *osfont, uint8_t color);

#endif
