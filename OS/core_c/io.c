#include <io.h>

void outb(unsigned short port, unsigned char value) {
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void outw(unsigned short port, unsigned short value) {
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

void outl(unsigned short port, unsigned int value) {
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}


unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

unsigned short inw(unsigned short port) {
	unsigned short ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

unsigned int inl(unsigned short port) {
	unsigned int ret;
	asm volatile ("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
