#ifndef __GFX_H_
#define __GFX_H_

#include <stdint.h>

volatile uint8_t back_buffer[320*200];

void set_pixel(int x, int y, uint8_t c);
void clear();
void flush();

#endif