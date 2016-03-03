#ifndef __MAP_H_
#define __MAP_H_

#include <stdint.h>
#include <sprite.h>

#define ISIZE 20.0

typedef struct tagOBJECT {
	SPRITE *sprite;
	float position[2];
	float hitbox[2];
	
	uint8_t a_frame;
	uint8_t animate;
} OBJECT;

typedef struct tagMAP {
	uint16_t width;
	uint16_t height;

	uint16_t sprite_count;
	
	OBJECT player;
	
	uint8_t follow_player;
	float view[2];
	
	uint8_t *palette;
	uint16_t *grid;
	SPRITE sprites[256];
} MAP;

uint16_t load_map(uint8_t *map_data, MAP *map);
uint16_t get_sprite_index(char *name, MAP *map);
uint8_t map_collision(float x, float y, OBJECT *object, MAP *map);
void draw_map(MAP *map);
#endif