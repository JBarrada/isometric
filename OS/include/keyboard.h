#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include <stdint.h>

# define KEY_ESC 		0x01

# define KEY_F1 		0x3B
# define KEY_F2 		0x3C
# define KEY_F3 		0x3D
# define KEY_F4 		0x3E
# define KEY_F5 		0x3F
# define KEY_F6 		0x40
# define KEY_F7 		0x41
# define KEY_F8 		0x42
# define KEY_F9 		0x43
# define KEY_F10		0x44
# define KEY_F11		0x57
# define KEY_F12		0x58

# define KEY_UP 		0x48
# define KEY_DOWN 		0x50
# define KEY_LEFT 		0x4B
# define KEY_RIGHT 		0x4D

# define KEY_L_SHIFT 	0x2A
# define KEY_R_SHIFT 	0x36
# define KEY_CTRL 		0x1D
# define KEY_ALT 		0x38

# define KEY_BACKSPACE 	0x0E
# define KEY_SPACE 		0x39
# define KEY_ENTER 		0x1C

#define RELEASE 0x80

void keyboard_irq();
void keyboard_init();
uint8_t keystate();

#endif
