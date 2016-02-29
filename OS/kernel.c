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

#include <map.h>
#include <sprite.h>

extern uint8_t _binary_game_MAPS_test_zipmap_start;
MAP map;

uint8_t update = 1;

void keystate_callback(uint8_t keystate) { 
	if ((state&1) == 1) {
		map.character_pos_top[0] -= 0.5;
		update = 1;
	}
	if (((state>>1)&1) == 1) {
		map.character_pos_top[0] += 0.5;
		update = 1;
	}
	if (((state>>2)&1) == 1) {
		map.character_pos_top[1] += 0.5;
		update = 1;
	}
	if (((state>>3)&1) == 1) {
		map.character_pos_top[1] -= 0.5;
		update = 1;
	}
}

//uint8_t* tb = (uint8_t*) 0xB8000;
/*
void debug_print(uint16_t x, uint16_t y, char *text){
	uint16_t offset = y*80+x;
	for (uint16_t i=0; i<strlen(text); i++) {
		tb[(offset+i)*2] = text[i];
	}
}
*/
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
	for (uint16_t c=0; c<(80*25); c++) {
		tb[c*2] = 0x00;
		tb[c*2+1] = 0x0f;
	}
	
	
	char debug[32];
	uint16_t y_pos = 0;
	
	for (int y=0; y<map.height; y++) {
		for (int x=0; x<map.width; x++) {
			uint16_t val = map.grid[y*map.width+x];
			itoa(val, 10, debug);
			debug_print(x, y_pos, debug);
			memset(debug, 0, 32);
		}
		y_pos++;
	}
	y_pos++;
	
	for (int i=0; i<map.sprite_count; i++) {
		itoa(map.sprites[i].height, 10, debug);
		debug_print(0, y_pos, debug);
		memset(debug, 0, 32);
		y_pos++;
	}
	*/
	
	//msleep(5000);
	
	
	vga_init(); //--------------------------------------
	
	clear();
	flush();
	
	map.character_pos_top[0] = 0;
	map.character_pos_top[1] = 0;
	
	for(;;) {
		/*
		uint8_t state = keystate();
		
		if ((state&1) == 1) {
			map.character_pos_top[0] -= 0.5;
			update = 1;
		}
		if (((state>>1)&1) == 1) {
			map.character_pos_top[0] += 0.5;
			update = 1;
		}
		if (((state>>2)&1) == 1) {
			map.character_pos_top[1] += 0.5;
			update = 1;
		}
		if (((state>>3)&1) == 1) {
			map.character_pos_top[1] -= 0.5;
			update = 1;
		}
		*/
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
