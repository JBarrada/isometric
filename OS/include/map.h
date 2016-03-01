#ifndef __MAP_H_
#define __MAP_H_

#include <stdint.h>
#include <sprite.h>

#define ISIZE 20.0

typedef struct tagMAP {
	uint16_t width;
	uint16_t height;

	uint16_t sprite_count;
	
	float character_pos_top[2];
	uint8_t direction;
	
	SPRITE *character;
	
	uint8_t *palette;
	
	uint16_t *grid;
	
	SPRITE sprites[256];
} MAP;

uint8_t load_map(uint8_t *map_data, MAP *map);
uint8_t map_collision(float x, float y, MAP *map);
void draw_map(MAP *map);
#endif