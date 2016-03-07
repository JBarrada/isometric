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

//static MAP cmap;
//static OSIMG controls;

extern uint8_t keystate;

void controls_keyboard(uint16_t key) {
	switch (key) {
		case KEY_ESC:
			esc = 1; break;
		default:
			return;
	}
}

uint8_t controls_move_player(MAP *cmap) { 
	if (DIR_KEYSTATE[keystate] != 0xff) {
		move_object(&cmap->player, cmap, DIR_KEYSTATE[keystate], 0.25);
	}
	return 1;
}

void controls_init() {
	/*
	load_map(&_binary_game_MAPS_controls_zipmap_start, &cmap);
	cmap.player.sprite = &cmap.sprites[get_sprite_index("CHAR_CART", &cmap)];
	cmap.follow_player = 0;
	
	cmap.player.position[0] = 3;
	cmap.player.position[1] = 3;
	cmap.player.hitbox[0] = 0.75;
	cmap.player.hitbox[1] = 0.75;
	
	cmap.view[0] = 0;
	cmap.view[1] = 0;
	cmap.c_offset[0] = -70;
	cmap.c_offset[1] = -30;
	*/
	//load_osimg(&_binary_game_IMAGES_controls_osimg_start, &controls);
}

uint8_t show_controls() {
	esc = 0;
	
	set_keyboard_callback(&controls_keyboard);
	
	gfx_clear_palette();
	
	MAP cmap;
	load_map(&_binary_game_MAPS_controls_zipmap_start, &cmap);
	cmap.player.sprite = &cmap.sprites[get_sprite_index("CHAR_CART", &cmap)];
	gfx_set_palette(cmap.palette);
	cmap.follow_player = 0;
	
	cmap.player.position[0] = 3;
	cmap.player.position[1] = 3;
	cmap.player.hitbox[0] = 0.75;
	cmap.player.hitbox[1] = 0.75;
	
	cmap.view[0] = 0;
	cmap.view[1] = 0;
	cmap.c_offset[0] = -70;
	cmap.c_offset[1] = -30;
	
	OSIMG controls;
	load_osimg(&_binary_game_IMAGES_controls_osimg_start, &controls);
	gfx_register_palette(controls.palette, controls.remap);
	
	uint8_t temp_palette[256*3];
	memcpy(temp_palette, gfx_get_palette(), 256*3);
	gfx_clear_palette();
	
	int fade_in=0;
	
	while (esc != 1) {
		controls_move_player(&cmap);
		
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
	uint8_t black[256*3];
	memset(black, 0, 256*3);
	fade_palette(black, 500);
	gfx_clear_palette();
	return 1;
}