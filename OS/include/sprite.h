#ifndef __SPRITE_H_
#define __SPRITE_H_

#include <stdint.h>

typedef struct tagSPRITE {
	uint16_t width;
	uint16_t height;

	uint16_t i_bot;
	uint16_t i_left;
	
	uint8_t t_z;
	uint8_t n_frames;
	
	uint8_t a_frame;
	uint8_t animate;
	
	char * name;
	uint8_t *bitmap;
} SPRITE;

uint8_t load_sprite(uint8_t *sprite_data, SPRITE *sprite);
void draw_sprite(int cx, int cy, SPRITE *sprite, uint8_t frame);

#endif