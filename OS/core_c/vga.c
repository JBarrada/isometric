#include <vga.h>
#include <io.h>
#include <stdint.h>

uint8_t g_320x200x256[] = {
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

uint8_t* front_buffer = (uint8_t*) 0xA0000;

void write_regs(uint8_t* regs) {
	unsigned i;

	// write MISCELLANEOUS reg
	outb(VGA_MISC_WRITE, *regs);
	regs++;
	
	// write SEQUENCER regs
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++) {
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
	
	// unlock CRTC registers
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
	
	// make sure they remain unlocked
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	
	// write CRTC regs
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++) {
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
	
	// write GRAPHICS CONTROLLER regs
	for(i = 0; i < VGA_NUM_GC_REGS; i++) {
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
	
	// write ATTRIBUTE CONTROLLER regs
	for(i = 0; i < VGA_NUM_AC_REGS; i++) {
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
	
	// lock 16-color palette and unblank display
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}

void set_palette_color(uint8_t color, uint8_t r, uint8_t g, uint8_t b) {
	outb(0x03C6, 0xff);
	outb(0x03C8, color);
	outb(0x03C9, r);
	outb(0x03C9, g);
	outb(0x03C9, b);
}

void set_palette(uint8_t* palette) {
	uint16_t num_colors = 256;
	for (uint16_t i = 0; i < num_colors; i++) {
		set_palette_color(i, palette[i*3+0], palette[i*3+1], palette[i*3+2]);
	}
}

void swap_buffers(uint8_t* back_buffer) {
	memcpy(front_buffer, back_buffer, 320*200);
}

void vga_init() {
	write_regs(g_320x200x256);
}