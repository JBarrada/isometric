#include <stdint.h>
#include <idt.h>
#include <io.h>
#include <keyboard.h>
#include <pic.h>
#include <pit.h>

uint8_t temp_keystate = 0;

void keyboard_init() {
	irq_clear_mask(1);
}

void keyboard_irq() {
	uint8_t sc = inb(0x60);
	if (sc == 0xe0) {
		while (sc == 0xe0)
			sc = inb(0x60);
	}
	switch (sc) {
		case KEY_LEFT:
			temp_keystate |= 0x01; break;
		case KEY_LEFT+RELEASE:
			temp_keystate &= 0xfe; break;
			
		case KEY_RIGHT:
			temp_keystate |= 0x02; break;
		case KEY_RIGHT+RELEASE:
			temp_keystate &= 0xfd; break;
			
		case KEY_UP:
			temp_keystate |= 0x04; break;
		case KEY_UP+RELEASE:
			temp_keystate &= 0xfb; break;
			
		case KEY_DOWN:
			temp_keystate |= 0x08; break;
		case KEY_DOWN+RELEASE:
			temp_keystate &= 0xf7; break;
	}
}

uint8_t keystate() {
	return temp_keystate;
}