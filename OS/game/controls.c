#include <stddef.h>
#include <stdint.h>

#include <pit.h>
#include <keyboard.h>
#include <io.h>
#include <gfx.h>
#include <osfont.h>

#include <map.h>
#include <sprite.h>
#include <osimg.h>

#include <random.h>
#include <string.h>
#include <debug.h>

#include <controls.h>

uint8_t esc = 0;

extern uint8_t _binary_game_MAPS_controls_zipmap_start;
extern uint8_t _binary_game_IMAGES_controls_osimg_start;

extern uint8_t keystate;

void controls_keyboard(uint16_t key) {
	switch (key) {
		case KEY_ESC:
			esc = 1; break;
		default:
			return;
	}
}

uint8_t controls_move_player() { 
	float *pos = cmap.player.position;
	
	float step = 0.25;
	float half = step/2;
	
	float steps[8][2] = {{-step, 0}, {-half, half}, {0, step}, {half, half}, {step, 0}, {half, -half}, {0, -step}, {-half, -half}};
	uint8_t frme[] = {4, 5, 6, 7, 0,  1, 2, 3};
	uint8_t dirs[] = {1, 5, 4, 6, 2, 10, 8, 9};
	//                l ul  u ur  r  dr  d dl
	
	for (uint8_t i=0; i<8; i++) {
		if (keystate == dirs[i]) {
			if (map_collision(pos[0]+steps[i][0], pos[1]+steps[i][1], &cmap.player, &cmap) == 0) {
				pos[0] += steps[i][0];
				pos[1] += steps[i][1];
				
				cmap.player.a_frame = frme[i];
				//update = 1;
				return 1;
			}
		}
	}
	return 1;
}

uint8_t show_controls() {
	MAP cmap;
	
	set_keyboard_callback(&controls_keyboard);
	
	gfx_clear_palette();
	load_map(&_binary_game_MAPS_controls_zipmap_start, &cmap);
	cmap.player.sprite = &cmap.sprites[get_sprite_index("CHAR_CART", &cmap)];
	gfx_set_palette(cmap.palette);
	cmap.follow_player = 0;
	
	cmap.player.position[0] = 3;
	cmap.player.position[1] = 3;
	cmap.player.hitbox[0] = 0.75;
	cmap.player.hitbox[1] = 0.75;
	
	cmap.view[0] = 3;
	cmap.view[1] = 3;
	cmap.c_offset[0] = 80;
	cmap.c_offset[1] = -10;
	
	OSIMG controls;
	load_osimg(&_binary_game_IMAGES_controls_osimg_start, &controls);
	gfx_register_palette(controls.palette, controls.remap);
	
	uint8_t temp_palette[256*3];
	memcpy(temp_palette, gfx_get_palette(), 256*3);
	gfx_clear_palette();
	
	int fade_in=0;
	
	for(;;) {
		if (esc == 1) {
			return 1;
		}
		controls_move_player();
		
		clear();
		draw_map(&cmap);
		draw_osimg(0, 0, &controls);
		flush();
	
		msleep(20);
		
		if (fade_in == 0) {
			fade_in = 1;
			fade_palette(temp_palette, 500);
		}
	}
}