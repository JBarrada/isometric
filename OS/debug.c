#include <stdint.h>

#include <vga.h>
#include <io.h>
#include <string.h>

#define TW 80
#define TH 25

uint8_t* terminal_buffer = (uint8_t*) 0xB8000;

void t_clear() {
	for (uint16_t c=0; c<(TW*TH); c++) {
		terminal_buffer[c*2] = 0x00;
		terminal_buffer[c*2+1] = 0x0f;
	}
}

void t_print(uint16_t x, uint16_t y, char *text) {
	uint16_t offset = y*80+x;
	for (uint16_t i=0; i<strlen(text); i++) {
		terminal_buffer[(offset+i)*2] = text[i];
	}
}