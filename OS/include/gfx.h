#ifndef __GFX_H_
#define __GFX_H_

#include <stdint.h>

void set_pixel(int x, int y, uint8_t c);
void circle_filled(int cx, int cy, int r, uint8_t c);
void clear();
void flush();

#endif