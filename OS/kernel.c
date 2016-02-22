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

#define isize 20.0

int tti(float x, float y, float z, float *ix, float *iy) {
    ix = (x+y)*0.866025403784;
    iy = z+(x-y)*0.5;
    return 1;
}

void kernel_main() {
	vga_init();
	
	gdt_init();
	idt_init();
	pic_init();
	
	pit_init();
	keyboard_init();
	
	asm("sti");
	
	clear();
	
	for (int x=0; x<10; x++) {
		for (int y=0; y<10; y++) {
			float ix, iy = 0, 0;
			tti((float)(x*ISIZE), (float)(y*ISIZE), 0, ix, iy);
			set_pixel(ix, iy, 0x32);
		}
	}
	
	flush();


	for(;;) {
		asm("hlt");
	}
}
