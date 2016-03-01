#ifndef __VGA_FONT_H_
#define __VGA_FONT_H_

int putch(int x, int y, char c, unsigned char color);
void putstr(int x, int y, char *data, unsigned char color);

#endif