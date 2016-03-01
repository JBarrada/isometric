#include <stdint.h>
#ifndef __VGA_H_
#define __VGA_H_

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF

#define VGA_CRTC_INDEX		0x3D4
#define VGA_CRTC_DATA		0x3D5
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21

void write_regs(uint8_t* regs);
void set_palette_color(uint8_t color, uint8_t r, uint8_t g, uint8_t b);
void set_palette(uint8_t* pal);
void swap_buffers(uint8_t* back_buffer);
void swap_line(uint8_t* buffer, uint16_t y);

void text_mode();

void vga_init();

#endif