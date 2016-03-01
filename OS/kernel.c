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

uint8_t update = 1;

void key_callback(uint8_t keystate) { 
	uint8_t direction = 0;

	float *pos = map.character_pos_top;
	
	float step_size = 0.125;
	float bounds = 1.0-step_size;
	
	if ((keystate&1) == 1) { // left
		if ((map_collision(pos[0]-step_size-bounds, pos[1], &map) == 0) && (map_collision(pos[0]-step_size-bounds, pos[1]+bounds, &map) == 0)) {
			pos[0] -= step_size;
			update = 1;
			direction |= 1;
		}
	}
	if (((keystate>>1)&1) == 1) { // right
		if ((map_collision(pos[0]+step_size, pos[1], &map) == 0) && (map_collision(pos[0]+step_size, pos[1]+bounds, &map) == 0)) {
			pos[0] += step_size;
			update = 1;
			direction |= 2;
		}
	}
	if (((keystate>>2)&1) == 1) { // up
		if ((map_collision(pos[0]-bounds, pos[1]+step_size+bounds, &map) == 0) && (map_collision(pos[0]-bounds, pos[1]+step_size+bounds, &map) == 0)) {
			pos[1] += step_size;
			update = 1;
			direction |= 4;
		}
	}
	if (((keystate>>3)&1) == 1) { // down
		if ((map_collision(pos[0], pos[1]-step_size, &map) == 0) && (map_collision(pos[0]-bounds, pos[1]-step_size, &map) == 0)) {
			pos[1] -= step_size;
			update = 1;
			direction |= 8;
		}
	}
	map.direction = direction;
}

void kernel_main() {
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	keyboard_init();
	
	set_keystate_callback(key_callback);
	
	asm("sti");
	
	load_map(&_binary_game_MAPS_test_zipmap_start, &map);
	set_palette(map.palette);
	
	t_clear();
	
	t_print(0, 0, map.character->name);
	
	for (int i=0; i<map.sprite_count; i++) {
		t_print(0, i+2, map.sprites[i].name);
		char debug[16];
		itoa(map.sprites[i].t_z, 10, debug);
		t_print(16, i+2, debug);
	}
	
	
	msleep(2000);
	vga_init();
	
	clear();
	flush();
	
	map.character_pos_top[0] = 1;
	map.character_pos_top[1] = 1;
	
	for(;;) {
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
