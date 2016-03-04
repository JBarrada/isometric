#ifndef __VGA_FONT_H_
#define __VGA_FONT_H_

#include <stdint.h>

#define b2b(b7,b6,b5,b4,b3,b2,b1,b0) ((uint8_t)((b7)*128u + (b6)*64u + (b5)*32u + (b4)*16u + (b3)*8u + (b2)*4u + (b1)*2u + (b0)))
typedef uint8_t TCDATA;
typedef TCDATA* TCLISTP;

extern TCLISTP smallfont[256];
extern TCLISTP axis[256];
extern TCLISTP axis_big[256];

#define SMALLFONT 0
#define AXIS 1
#define AXIS_BIG 2


int putch(int x, int y, char c, uint8_t color, uint8_t font);
void putstr(int x, int y, char *data, uint8_t color, uint8_t font);

#endif