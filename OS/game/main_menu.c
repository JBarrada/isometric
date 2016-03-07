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

#include <main_menu.h>

char menuwords[][32] = {"APOLOGY", "SCATOLOGY", "SCIENTOLOGY", "GYNECOLOGY", "BROCCOLI", "IMPROBABLY", "MONOPOLY", "GEOMETRY", "DICHOTOMY", "CRIMINOLOGY", "DOXOLOGY"};

extern uint8_t _binary_game_IMAGES_MENUBOX_osimg_start;
extern uint8_t _binary_game_IMAGES_DETENTELOGO_osimg_start;
extern uint8_t _binary_game_IMAGES_X1_osimg_start;
extern uint8_t _binary_game_IMAGES_X2_osimg_start;
extern uint8_t _binary_game_IMAGES_X3_osimg_start;

uint8_t icon = 0;
int selected = 0;
uint8_t enter = 0;

//static OSIMG menubox, detentelogo, x1, x2, x3;

void menu_keyboard(uint16_t key) {
	switch (key) {
		case KEY_LEFT:
			selected -= 1; break;
		case KEY_RIGHT:
			selected += 1; break;
		case KEY_UP:
			selected -= 1; break;	
		case KEY_DOWN:
			selected += 1; break;
		
		case KEY_ENTER:
			enter = 1; break;
		default:
			return;
	}
	
	if ((selected<0) || (selected>2)) {
		if (selected>2) selected=2;
		if (selected<0) selected=0;
	} else {
		uint32_t temp = icon;
		while (icon==temp)
			icon = random_number(0, 3);
	}
}

uint8_t show_main_menu() {
	enter = 0;
	selected = 0;
	icon = 0;
	
	set_keyboard_callback(&menu_keyboard);
	
	gfx_clear_palette();
	
	OSIMG menubox, detentelogo, x1, x2, x3;
	
	load_osimg(&_binary_game_IMAGES_MENUBOX_osimg_start, &menubox);
	load_osimg(&_binary_game_IMAGES_DETENTELOGO_osimg_start, &detentelogo);
	
	load_osimg(&_binary_game_IMAGES_X1_osimg_start, &x1);
	load_osimg(&_binary_game_IMAGES_X2_osimg_start, &x2);
	load_osimg(&_binary_game_IMAGES_X3_osimg_start, &x3);
	
	gfx_clear_palette();
	gfx_register_palette(menubox.palette, menubox.remap);
	gfx_register_palette(detentelogo.palette, detentelogo.remap);
	gfx_register_palette(x1.palette, x1.remap);
	gfx_register_palette(x2.palette, x2.remap);
	gfx_register_palette(x3.palette, x3.remap);
	
	int white_index = gfx_register_color((uint8_t[3]){63, 63, 63});
	
	uint8_t temp_palette[256*3];
	memcpy(temp_palette, gfx_get_palette(), 256*3);
	gfx_clear_palette();
	
	int word=0, word_timer=0, menu_timer=0, fade_in=0;
	while (enter != 1) {
		clear();
		draw_osimg(95, 30, &menubox);
		draw_osimg(80, 155, &detentelogo);
		
		uint8_t iadj[3] = {8, 12, 8};
		OSIMG *icons[3] = {&x1, &x2, &x3};
		draw_osimg(95+50,30+iadj[icon]+((2-selected)*15), icons[icon]);
		
		putstr(230-fontstrlen(menuwords[word], &AXIS_S), 140, menuwords[word], &AXIS_S, white_index);
		
		flush();
		msleep(20);
		
		if (word_timer++>200) {
			word_timer = 0;
			int temp = word;
			while (word==temp)
				word = random_number(0, 11);
		}
		if (menu_timer++>30000) {
			menu_timer = 0;
		}
		if (fade_in == 0) {
			fade_in = 1;
			fade_palette(temp_palette, 500);
		}
	}
	uint8_t black[256*3];
	memset(black, 0, 256*3);
	fade_palette(black, 500);
	gfx_clear_palette();
	return selected;
}