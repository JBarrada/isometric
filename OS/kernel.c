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

#define ISIZE 20.0

extern uint8_t _binary_game_MAPS_test_zipmap_start;

int tti(float x, float y, float z, float* iso) {
    iso[0] = (x+y)*(float)0.866025403784;
    iso[1] = z+(x-y)*(float)0.5;
    return 1;
}

void kernel_main() {
	vga_init();
	
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	//keyboard_init();
	
	asm("sti");
	
	
	clear();
	
	MAP map;
	load_map(&_binary_game_MAPS_test_zipmap_start, &map);
	
	for (int x=0; x<map.width; x++) {
		for (int y=0; y<map.height; y++) {
			float iso[2];
			tti((float)(x*ISIZE), (float)(y*ISIZE), 0, iso);
			set_pixel(iso[0]+160, iso[1]+100, 255);
		}
	}
	
	draw_sprite(160, 100, &map.sprites[0]);
	
	flush();
	
	set_palette(map.palette);
	
	//draw_sprite(200, 100, &map.sprites[1]);
	
	//flush();
	
	for(;;) {
		asm("hlt");
	}
}
