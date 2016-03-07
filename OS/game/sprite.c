#include <io.h>
#include <stdint.h>
#include <gfx.h>
#include <sprite.h>

uint8_t load_sprite(uint8_t *sprite_data, SPRITE *sprite) {
	memset(sprite, 0, sizeof(SPRITE));
	sprite->width = sprite_data[0]|(sprite_data[1]<<8);
	sprite->height = sprite_data[2]|(sprite_data[3]<<8);
	
	sprite->i_bot = sprite_data[4]|(sprite_data[5]<<8);
	sprite->i_left = sprite_data[6]|(sprite_data[7]<<8);
	
	sprite->t_z = sprite_data[8];
	sprite->n_frames = sprite_data[9];
	
	sprite->name = (char*)&sprite_data[10];

	sprite->bitmap = &sprite_data[26];
	
	return 1;
}

void draw_sprite(int cx, int cy, SPRITE *sprite, uint8_t frame) {
	int o_frame = (sprite->height*sprite->width)*(sprite->n_frames-frame-1);
	int ox=cx-sprite->i_left, oy=cy-sprite->i_bot;
	
	for (uint16_t y=0; y<sprite->height; y++) {
		for (uint16_t x=0; x<sprite->width; x++) {
			uint8_t index = sprite->bitmap[y*sprite->width+x+o_frame];
			if (index != 0xff) {
				set_pixel(ox+x, oy+y, index);
			}
		}
	}
}