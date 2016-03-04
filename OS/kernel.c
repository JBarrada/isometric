#define KERNEL

#include <stddef.h>
#include <stdint.h>

#include <keyboard.h>
#include <io.h>

#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>

#include <vga.h>
#include <gfx.h>
#include <string.h>
#include <debug.h>

#include <map.h>
#include <sprite.h>
#include <main_menu.h>

#include <random.h>

extern uint8_t _binary_game_MAPS_test_zipmap_start;
MAP map;

extern uint8_t keystate;
uint8_t update = 1;

uint8_t move_player() { 
	float *pos = map.player.position;
	
	float step = 0.25;
	float half = step/2;
	
	float steps[8][2] = {{-step, 0}, {-half, half}, {0, step}, {half, half}, {step, 0}, {half, -half}, {0, -step}, {-half, -half}};
	uint8_t frme[] = {4, 5, 6, 7, 0,  1, 2, 3};
	uint8_t dirs[] = {1, 5, 4, 6, 2, 10, 8, 9};
	//                l ul  u ur  r  dr  d dl
	
	for (uint8_t i=0; i<8; i++) {
		if (keystate == dirs[i]) {
			if (map_collision(pos[0]+steps[i][0], pos[1]+steps[i][1], &map.player, &map) == 0) {
				pos[0] += steps[i][0];
				pos[1] += steps[i][1];
				
				map.player.a_frame = frme[i];
				update = 1;
				return 1;
			}
		}
	}
	return 1;
}

void kernel_main() {
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	keyboard_init();

	asm("sti");
	
	/*
	t_clear();
	for (int i=0; i<25; i++) {
		char debug[32];
		itoa(menubox.remap[i], 10, debug);
		t_print(0, i, debug);
		memset(debug, 0, 32);
		
		itoa(menubox.palette[i*3+0], 16, debug);
		t_print(0+4, i, debug);
		memset(debug, 0, 32);
		itoa(menubox.palette[i*3+1], 16, debug);
		t_print(3+4, i, debug);
		memset(debug, 0, 32);
		itoa(menubox.palette[i*3+2], 16, debug);
		t_print(6+4, i, debug);
		memset(debug, 0, 32);
		
	}
	*/
	
	vga_init();
	
	show_main_menu();
	
	/*
	load_map(&_binary_game_MAPS_test_zipmap_start, &map);
	map.player.sprite = &map.sprites[get_sprite_index("CHAR_CART", &map)];
	map.follow_player = 1;
	
	map.player.position[0] = 3;
	map.player.position[1] = 35;
	map.player.hitbox[0] = 0.75;
	map.player.hitbox[1] = 0.75;
	
	
	vga_init();
	
	clear();
	flush();
	
	uint32_t passed = 0;
	uint8_t black[256*3];
	memset(black, 0, 256*3);
	
	gfx_set_palette(black);
	
	for(;;) {
		if (passed == 1) fade_palette(map.palette, 250);
		
		move_player();
		if (update) {
			clear();
			draw_map(&map);
			flush();
			update = 0;
		}
		msleep(20);
		passed++;
	}
	*/
	
	for(;;) {
		asm("hlt");
	}
}
