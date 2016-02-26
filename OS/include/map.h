#ifndef __MAP_H_
#define __MAP_H_

#include <stdint.h>
#include <sprite.h>

typedef struct tagMAP {
	uint16_t width;
	uint16_t height;

	uint16_t sprite_count;
	
	uint8_t *palette;
	
	uint16_t *grid;
	
	SPRITE *sprites;
} MAP;

uint8_t load_map(uint8_t *map_data, MAP *map);
#endif