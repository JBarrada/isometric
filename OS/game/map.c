#include <stdint.h>
#include <gfx.h>
#include <vga.h>
#include <sprite.h>
#include <map.h>

uint8_t load_map(uint8_t *map_data, MAP *map) {
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
	
	return 1;
}

int tti(float x, float y, float z, float* iso) {
	y *= -1.0;
    iso[0] = (x-y)*(float)0.866025403784;
    iso[1] = -(z+(x+y)*(float)0.5);
    return 1;
}

void draw_map(MAP *map) {
	float character_pos_iso[2];
	tti(map->character_pos_top[0]*ISIZE, map->character_pos_top[1]*ISIZE, 0, character_pos_iso);
	
	int ox = 160-character_pos_iso[0];
	int oy = 100-character_pos_iso[1];
	
	for (int y=(map->height-1); y>=0; y--) {
		for (int x=0; x<map->width; x++) {
			float iso[2];
			tti((float)(x*ISIZE), (float)(y*ISIZE), 0, iso);
			uint16_t tile = map->grid[y*map->width+x];
			if (tile != 0xffff) {
				if ((ox+iso[0] < 350) && (ox+iso[0] > -30) && (oy+iso[1] < 230) && (oy+iso[1] > -100)) {
					draw_sprite(iso[0]+ox, iso[1]+oy, &map->sprites[tile]);
				}
			}
			if (((int)map->character_pos_top[0] == (x+1)) && ((int)map->character_pos_top[1] == (y+1))) {
				circle_filled(160, 100, 8, 255);
			}
		}
	}
	
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