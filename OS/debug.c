#include <stdint.h>

#include <vga.h>
#include <io.h>
#include <string.h>

#define TW 80
#define TH 25

uint8_t* terminal_buffer = (uint8_t*) 0xB8000;

void t_clear() {
	for (uint16_t c=0; c<(TW*TH); c++) {
		tb[c*2] = 0x00;
		tb[c*2+1] = 0x0f;
	}
}



