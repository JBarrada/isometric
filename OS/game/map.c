#include <io.h>
#include <stdint.h>
#include <string.h>
#include <gfx.h>
#include <vga.h>
#include <sprite.h>
#include <map.h>

#include <vga_font.h>

uint16_t load_map(uint8_t *map_data, MAP *map) {
	map->width = map_data[0]|(map_data[1]<<8);
	map->height = map_data[2]|(map_data[3]<<8);
	
	uint32_t grid_start = map_data[4]|(map_data[5]<<8)|(map_data[6]<<16)|(map_data[7]<<24);
	map->grid = (uint16_t*)&map_data[grid_start];
	
	map->palette = &map_data[8];
	
	map->sprite_count = map_data[776]|(map_data[777]<<8);
	
	for (uint32_t i=0; i<map->sprite_count; i++) {
		uint32_t sprite_start = map_data[778+(i*4)]|(map_data[779+(i*4)]<<8)|(map_data[780+(i*4)]<<16)|(map_data[781+(i*4)]<<24);
		load_sprite(&map_data[sprite_start], &map->sprites[i]);
	}
	
	return map->sprite_count;
}

uint16_t get_sprite_index(char *name, MAP *map) {
	for (uint16_t i=0; i<map->sprite_count; i++) {
		if (strcompare(name, map->sprites[i].name) == 1) {
			return i;
		}
	}
	return -1;
}

int tti(float x, float y, float z, float* iso) {
	y *= -1.0;
    iso[0] = (x-y)*(float)0.866025403784;
    iso[1] = -(z+(x+y)*(float)0.5);
    return 1;
}

uint8_t map_collision(float x, float y, OBJECT *object, MAP *map) {
	float *hb = object->hitbox;
	float pos[4][2] = {{x, y}, {x-hb[0], y}, {x-hb[0], y+hb[1]}, {x, y+hb[1]}};
	for (uint8_t i=0; i<4; i++) {
		int g_index = ((int)pos[i][1]*map->width)+(int)pos[i][0];
		if (map->sprites[map->grid[g_index]].t_z != 0)
			return 1;
	}
	return 0;
}

void draw_map(MAP *map) {
	int ox, oy;
	float view_iso[2];
	if (map->follow_player == 1) {
		tti(map->player.position[0]*ISIZE, map->player.position[1]*ISIZE, 0, view_iso);
	} else {
		tti(map->view[0]*ISIZE, map->view[1]*ISIZE, 0, view_iso);
	}
	ox = 160-view_iso[0];
	oy = 100-view_iso[1];
	

	for (int x=0; x<map->width; x++) {
		for (int y=(map->height-1); y>=(int)map->player.position[1]; y--) {
			float iso[2];
			tti((float)((x+1)*ISIZE), (float)(y*ISIZE), 0, iso);
			uint16_t tile = map->grid[y*map->width+x];
			if (tile != 0xffff) {
				if ((ox+iso[0] < 350) && (ox+iso[0] > -30) && (oy+iso[1] < 230) && (oy+iso[1] > -100)) {
					draw_sprite(iso[0]+ox, iso[1]+oy, &map->sprites[tile], 0);
				}
			}
			if (((int)map->player.position[0] == (x)) && ((int)map->player.position[1] == (y))) {
				float player_iso[2];
				tti((float)(map->player.position[0]*ISIZE), (float)(map->player.position[1]*ISIZE), 0, player_iso);
				draw_sprite(player_iso[0]+ox, player_iso[1]+oy, map->player.sprite, map->player.a_frame);
			}
		}
	}
	for (int y=(int)map->player.position[1]-1; y>=0; y--) {
		for (int x=0; x<map->width; x++) {
			float iso[2];
			tti((float)((x+1)*ISIZE), (float)(y*ISIZE), 0, iso);
			uint16_t tile = map->grid[y*map->width+x];
			if (tile != 0xffff) {
				if ((ox+iso[0] < 350) && (ox+iso[0] > -30) && (oy+iso[1] < 230) && (oy+iso[1] > -100)) {
					draw_sprite(iso[0]+ox, iso[1]+oy, &map->sprites[tile], 0);
				}
			}
		}
	}
	
	
	char debug[16];
	itoa((int)map->player.position[0], 10, debug);
	putstr(16, 0, debug, 255);
	memset(debug, 0, 16);
	itoa((int)map->player.position[1], 10, debug);
	putstr(16, 8, debug, 255);
	
	putstr(0, 0, "X", 255);
	putstr(0, 8, "Y", 255);

	/*
	float target_iso[2];
	tti((float)(map->character_pos_top[0]*ISIZE), (float)(map->character_pos_top[1]*ISIZE), 0, target_iso);
	set_pixel(target_iso[0]+ox, target_iso[1]+oy, 255);
	tti((float)((map->character_pos_top[0]-1)*ISIZE), (float)((map->character_pos_top[1])*ISIZE), 0, target_iso);
	set_pixel(target_iso[0]+ox, target_iso[1]+oy, 255);
	tti((float)((map->character_pos_top[0]-1)*ISIZE), (float)((map->character_pos_top[1]+1)*ISIZE), 0, target_iso);
	set_pixel(target_iso[0]+ox, target_iso[1]+oy, 255);
	tti((float)((map->character_pos_top[0])*ISIZE), (float)((map->character_pos_top[1]+1)*ISIZE), 0, target_iso);
	set_pixel(target_iso[0]+ox, target_iso[1]+oy, 255);
	*/
	
	/*
	for (int gx=0; gx<map->width; gx++) {
		for (int gy=0; gy<map->width; gy++) {
			float iso[2];
			tti((float)(gx*ISIZE), (float)(gy*ISIZE), 0, iso);
			set_pixel(iso[0]+ox, iso[1]+oy, 255);
		}
	}
	*/
}