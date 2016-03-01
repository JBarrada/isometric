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

extern uint8_t _binary_game_MAPS_test_zipmap_start;
MAP map;

extern uint8_t keystate;
uint8_t update = 1;

void key_callback() { 
	float *pos = map.character_pos_top;
	
	float step_size = 0.25;
	float bounds = 1.0-step_size;
	
	if ((keystate&1) == 1) { // left
		if ((map_collision(pos[0]-step_size-bounds, pos[1], &map) == 0) && (map_collision(pos[0]-step_size-bounds, pos[1]+bounds, &map) == 0)) {
			if (keystate == 1) {
				pos[0] -= step_size;
			} else {
				pos[0] -= (step_size/2);
			}
			update = 1;
		}
	}
	if (((keystate>>1)&1) == 1) { // right
		if ((map_collision(pos[0]+step_size, pos[1], &map) == 0) && (map_collision(pos[0]+step_size, pos[1]+bounds, &map) == 0)) {
			if (keystate == 2) {
				pos[0] += step_size;
			} else {
				pos[0] += (step_size/2);
			}
			update = 1;
		}
	}
	if (((keystate>>2)&1) == 1) { // up
		if ((map_collision(pos[0], pos[1]+step_size+bounds, &map) == 0) && (map_collision(pos[0]-bounds, pos[1]+step_size+bounds, &map) == 0)) {
			if (keystate == 4) {
				pos[1] += step_size;
			} else {
				pos[1] += (step_size/2);
			}
			update = 1;
		}
	}
	if (((keystate>>3)&1) == 1) { // down
		if ((map_collision(pos[0], pos[1]-step_size, &map) == 0) && (map_collision(pos[0]-bounds, pos[1]-step_size, &map) == 0)) {
			if (keystate == 8) {
				pos[1] -= step_size;
			} else {
				pos[1] -= (step_size/2);
			}
			update = 1;
		}
	}
	map.direction = keystate; //direction;
}

void kernel_main() {
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	keyboard_init();

	asm("sti");
	
	load_map(&_binary_game_MAPS_test_zipmap_start, &map);
	set_palette(map.palette);
	
	/*
	t_clear();
	
	t_print(0, 0, map.character->name);
	
	for (int i=0; i<map.sprite_count; i++) {
		t_print(0, i+2, map.sprites[i].name);
		char debug[16];
		itoa(map.sprites[i].t_z, 10, debug);
		t_print(16, i+2, debug);
	}
	
	msleep(1000);
	*/
	
	vga_init();
	
	clear();
	flush();
	
	map.character_pos_top[0] = 3;
	map.character_pos_top[1] = 35;
	
	for(;;) {
		key_callback();
		if (update) {
			clear();
			draw_map(&map);
			flush();
			update = 0;
			msleep(20);
		}
	}
	
	
	for(;;) {
		asm("hlt");
	}
}
