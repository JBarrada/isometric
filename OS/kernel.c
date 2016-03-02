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

uint8_t move_player() { 
	float *pos = map.player.position;
	
	float step = 0.25;
	float half = step/2;
	
	float steps[] = {{-step, 0}, {-half, half}, {0, step}, {half, half}, {step, 0}, {half, -half}, {0, -step}, {-half, -half}};
	uint8_t frme[] = {4, 5, 6, 7, 0,  1, 2, 3};
	uint8_t dirs[] = {1, 5, 4, 6, 2, 10, 8, 9};
	//                l ul  u ur  r  dr  d dl
	
	for (uint8_t i=0; i<8; i++) {
		if (keystate == dirs[i]) {
			if (map_collision(pos[0]+steps[i][0], pos[1]+steps[i][1], &map.player) == 0) {
				pos[0] += steps[i][0];
				pos[1] += steps[i][1];
				
				map.player.a_frame = frme[i];
				update = 1;
				return 1;
			}
		}
	}
	/*
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
	*/
}

void kernel_main() {
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	keyboard_init();

	asm("sti");
	
	load_map(&_binary_game_MAPS_test_zipmap_start, &map);
	set_sprite("CHAR_CART", map.player.sprite);
	set_palette(map.palette);
	
	map.player.position[0] = 3;
	map.player.position[1] = 35;
	map.player.hitbox[0] = 0.75;
	map.player.hitbox[1] = 0.75;
	
	
	vga_init();
	
	clear();
	flush();
	
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
