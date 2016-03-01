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
	if ((keystate&1) == 1) {
		map.character_pos_top[0] -= 0.125;
		update = 1;
	}
	if (((keystate>>1)&1) == 1) {
		map.character_pos_top[0] += 0.125;
		update = 1;
	}
	if (((keystate>>2)&1) == 1) {
		map.character_pos_top[1] += 0.125;
		update = 1;
	}
	if (((keystate>>3)&1) == 1) {
		map.character_pos_top[1] -= 0.125;
		update = 1;
	}
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
	
	//t_clear();
	//t_print(0, 0, "HELLO");
	
	
	vga_init();
	
	clear();
	flush();
	
	map.character_pos_top[0] = 10;
	map.character_pos_top[1] = 10;
	
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
