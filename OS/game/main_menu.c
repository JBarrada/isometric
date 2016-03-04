#include <stddef.h>
#include <stdint.h>

#include <pit.h>
#include <keyboard.h>
#include <io.h>
#include <gfx.h>
#include <vga_font.h>

#include <map.h>
#include <sprite.h>
#include <osimg.h>

#include <random.h>


extern uint8_t _binary_game_IMAGES_MENUBOX_osimg_start;
extern uint8_t _binary_game_IMAGES_DETENTELOGO_osimg_start;
extern uint8_t _binary_game_IMAGES_X1_osimg_start;
extern uint8_t _binary_game_IMAGES_X2_osimg_start;
extern uint8_t _binary_game_IMAGES_X3_osimg_start;

uint8_t icon = 0;
int selected = 0;

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
	set_keyboard_callback(&menu_keyboard);
	
	OSIMG menubox, detentelogo, x1, x2, x3;
	load_osimg(&_binary_game_IMAGES_MENUBOX_osimg_start, &menubox);
	gfx_clear_palette();
	gfx_register_palette(menubox.palette, menubox.remap);
	
	load_osimg(&_binary_game_IMAGES_DETENTELOGO_osimg_start, &detentelogo);
	gfx_register_palette(detentelogo.palette, detentelogo.remap);
	
	load_osimg(&_binary_game_IMAGES_X1_osimg_start, &x1);
	gfx_register_palette(x1.palette, x1.remap);
	load_osimg(&_binary_game_IMAGES_X2_osimg_start, &x2);
	gfx_register_palette(x2.palette, x2.remap);
	load_osimg(&_binary_game_IMAGES_X3_osimg_start, &x3);
	gfx_register_palette(x3.palette, x3.remap);
	
	for (;;) {
		clear();
		draw_osimg(95, 40, &menubox);
		draw_osimg(80, 155, &detentelogo);
		
		uint8_t iadj[3] = {8, 12, 8};
		OSIMG *icons[3] = {&x1, &x2, &x3};
		draw_osimg(95+50,40+iadj[icon]+((2-selected)*15), icons[icon]);
		
		putstr(160, 100, "TEST", 255, 2);
		
		flush();
		msleep(20);
	}

	
	return 1;
}