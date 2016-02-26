#include <stdint.h>
#include <gfx.h>
#include <vga.h>
#include <sprite.h>
#include <map.h>

uint8_t load_map(uint8_t *map_data, MAP *map) {
	map->width = map_data[0]|(map_data[1]<<8);
	map->height = map_data[2]|(map_data[3]<<8);
	
	uint16_t grid_start = map_data[4]|(map_data[5]<<8);
	//map->grid = &map_data[grid_start];
	
	map->palette = &map_data[6];
	
	uint16_t sprite_count = map_data[774]|(map_data[775]<<8);
	SPRITE sprites[2];
	
	for (uint16_t i=0; i<2; i++) {
		uint16_t sprite_start = map_data[776+(i*2)]|(map_data[777+(i*2)]<<8);
		SPRITE tempsprite;
		load_sprite((uint8_t*)&map_data[sprite_start], &tempsprite);
		sprites = &tempsprite;
	}
	
	map->sprites = sprites;
	
	return 1;
}