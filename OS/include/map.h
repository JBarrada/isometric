#ifndef __MAP_H_
#define __MAP_H_

#include <stdint.h>
#include <sprite.h>

#define D_L 	1
#define D_UL 	5
#define D_U 	4
#define D_UR 	6
#define D_R 	2
#define D_DR 	10
#define D_D 	8
#define D_DL 	9

#define ISIZE 20.0

extern const uint8_t FRAMES[8];
extern const uint8_t DIR_KEYSTATE[16];
extern const uint8_t DIRECTIONS[8];
extern const float DIR_STEP[8][2];

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
	float c_offset[2];
	
	uint8_t *palette;
	uint16_t *grid;
	SPRITE sprites[256];
} MAP;

uint16_t load_map(uint8_t *map_data, MAP *map);
uint16_t get_sprite_index(char *name, MAP *map);
uint8_t map_collision(float x, float y, OBJECT *object, MAP *map);
uint8_t move_object(OBJECT *object, MAP *map, uint8_t dir, float step);
void draw_map(MAP *map);
#endif