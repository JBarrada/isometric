#ifndef IO
#define IO 1

extern void * memcpy ( void * destination, const void * source, unsigned int num);
extern void * memset ( void * ptr, int value, unsigned int num );

void outb(unsigned short port,unsigned char value);
void outw(unsigned short port,unsigned short value);
void outl(unsigned short port,unsigned int value);
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
unsigned int inl(unsigned short port);

#endif
