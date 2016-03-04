#include <stdint.h>
#include <gfx.h>
#include <osimg.h>

uint8_t load_osimg(uint8_t *osimg_data, OSIMG *osimg) {
	osimg->width = osimg_data[0]|(osimg_data[1]<<8);
	osimg->height = osimg_data[2]|(osimg_data[3]<<8);
	
	osimg->palette = &osimg_data[4];
	osimg->bitmap = &osimg_data[836];
	return 1;
}

void draw_osimg(int ox, int oy, OSIMG *osimg) {
	for (uint16_t y=0; y<osimg->height; y++) {
		for (uint16_t x=0; x<osimg->width; x++) {
			uint8_t index = osimg->remap[osimg->bitmap[y*osimg->width+x]];
			if (index != 0xff) {
				set_pixel(ox+x, oy+y, index);
			}
		}
	}
}