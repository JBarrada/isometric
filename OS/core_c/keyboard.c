#include <stdint.h>
#include <idt.h>
#include <io.h>
#include <keyboard.h>
#include <pic.h>
#include <pit.h>

// completely redo this

uint16_t scan_code = 0;
uint8_t new_scancode = 0;

uint8_t l_shift_pressed = 0;
uint8_t r_shift_pressed = 0;
uint8_t ctrl_pressed = 0;
uint8_t alt_pressed = 0;

const char chars[84] = {'\0', 0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
const char chars_shift[84] = {'\0', 0x1b, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0', '\0', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

void keyboard_init() {
	irq_clear_mask(1);
}

char key_to_char(uint16_t key) {
	char ret = '\0';
	if (key < 84) {
		if (l_shift_pressed || r_shift_pressed)
			ret = chars_shift[key];
		else
			ret = chars[key];
	}
	return ret;
}

uint16_t get_key() {
	new_scancode = 0;
	while (1) {
		if (new_scancode) {
			new_scancode = 0;
			return scan_code;
		}
	}
}

char get_char() {
	new_scancode = 0;
	char ascii;
	while (1) {
		if (new_scancode) {
			new_scancode = 0;
			ascii = key_to_char(scan_code);
			if (ascii != 0)
				return ascii;
		}
	}
}

void keyboard_irq()
{
	uint8_t sc = inb(0x60);
	if (sc == 0xe0) {
		while (sc == 0xe0)
			sc = inb(0x60);
		scan_code = 0xe000 | sc;
	}
	else {
		scan_code = sc;
		switch (sc) {
			case L_SHIFT_PRESSED:
				l_shift_pressed = 1; break;
			case L_SHIFT_RELEASED:
				l_shift_pressed = 0; break;
			case R_SHIFT_PRESSED:
				r_shift_pressed = 1; break;
			case R_SHIFT_RELEASED:
				r_shift_pressed = 0; break;
				
			case CTRL_PRESSED:
				ctrl_pressed = 1; break;
			case CTRL_RELEASED:
				ctrl_pressed = 0; break;
				
			case ALT_PRESSED:
				alt_pressed = 1; break;
			case ALT_RELEASED:
				alt_pressed = 0; break;
		} 
	}
	new_scancode = 1;
}
