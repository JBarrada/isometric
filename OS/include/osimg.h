#ifndef __OSIMG_H_
#define __OSIMG_H_

typedef struct tagOSIMG {
	uint16_t width;
	uint16_t height;
	
	uint8_t remap[256];
	uint8_t *palette;
	uint8_t *bitmap;
} OSIMG;

uint8_t load_osimg(uint8_t *osimg_data, OSIMG *osimg);
void draw_osimg(int ox, int oy, OSIMG *osimg);
#endif