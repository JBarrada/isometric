#include <stddef.h>
#include <stdint.h>

#include <pit.h>
#include <keyboard.h>
#include <io.h>
#include <gfx.h>

#include <map.h>
#include <sprite.h>

extern uint8_t _binary_game_MAPS_main_menu_zipmap_start;
MAP mainmenu_map;

uint8_t show_main_menu() {
	load_map(&_binary_game_MAPS_main_menu_zipmap_start, &mainmenu_map);
	mainmenu_map.player.sprite = &mainmenu_map.sprites[get_sprite_index("CHAR_CART", &mainmenu_map)];
	
	mainmenu_map.player.position[0] = 3;
	mainmenu_map.player.position[1] = 2;
	mainmenu_map.follow_player = 0;
	
	mainmenu_map.view[0] = 4;
	mainmenu_map.view[1] = 5;
	
	uint8_t black_palette[256*3];
	memset(black_palette, 0, 256*3);
	
	gfx_set_palette(black_palette);
	
	clear();
	draw_map(&mainmenu_map);
	flush();
	
	fade_palette(mainmenu_map.palette, 250);
	
	//uint8_t frme[] = {4, 5, 6, 7, 0,  1, 2, 3};
	//uint8_t dirs[] = {1, 5, 4, 6, 2, 10, 8, 9};
	//                  l ul  u ur  r  dr  d dl
	
	// 3,2 6,2 6,6 3,6
	int i = 0;
	for (;;) {
		mainmenu_map.player.a_frame = 1;
		for (i=0; i<20; i++) {
			clear();
			draw_map(&mainmenu_map);
			flush();
			msleep(20);
			// move right
			mainmenu_map.player.a_frame = 0;
			mainmenu_map.player.position[0] += 0.2;
		}
		
		mainmenu_map.player.a_frame = 7;
		for (i=0; i<20; i++) {
			clear();
			draw_map(&mainmenu_map);
			flush();
			msleep(20);
			// move up
			mainmenu_map.player.a_frame = 6;
			mainmenu_map.player.position[1] += 0.2;
		}
		
		mainmenu_map.player.a_frame = 5;
		for (i=0; i<20; i++) {
			clear();
			draw_map(&mainmenu_map);
			flush();
			msleep(20);
			// move left
			mainmenu_map.player.a_frame = 4;
			mainmenu_map.player.position[0] -= 0.2;
		}
		
		mainmenu_map.player.a_frame = 3;
		for (i=0; i<20; i++) {
			clear();
			draw_map(&mainmenu_map);
			flush();
			msleep(20);
			// move down
			mainmenu_map.player.a_frame = 2;
			mainmenu_map.player.position[1] -= 0.2;
		}
	}
}