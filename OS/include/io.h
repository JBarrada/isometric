#ifndef IO
#define IO 1

#include <stdint.h>

extern void * memcpy (void * destination, const void * source, uint32_t num);
extern void * memset (void * ptr, int value, uint32_t num);

void outb(uint16_t port,uint8_t value);
void outw(uint16_t port,uint16_t value);
void outl(uint16_t port,uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

#endif
