#include <pic.h>
#include <io.h>
#include <stdint.h>

void pic_init() {
	// save current masks, set by BIOS
	// set up cascading mode
	outb(PIC_MASTER_CMD, 0x10 + 0x01);
	outb(PIC_SLAVE_CMD,  0x10 + 0x01);
	// setup master's vector offset
	outb(PIC_MASTER_DATA, 0x20);
	// tell the slave its vector offset
	outb(PIC_SLAVE_DATA, 0x28);
	// tell the master that he has a slave
	outb(PIC_MASTER_DATA, 4);
	outb(PIC_SLAVE_DATA, 2);
	// enabled 8086 mode
	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);

	outb(PIC_MASTER_DATA, 0xff);
	outb(PIC_SLAVE_DATA, 0xff);
}

void irq_set_mask(unsigned char irq) {
    uint16_t port;
    uint8_t value;
 
    if(irq < 8)
        port = PIC_MASTER_DATA;
    else {
        port = PIC_SLAVE_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);        
}
 
void irq_clear_mask(unsigned char irq) {
    uint16_t port;
    uint8_t value;
 
    if(irq < 8)
        port = PIC_MASTER_DATA;
    else {
        port = PIC_SLAVE_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);        
}

void pic_send_eoi(uint8_t vector) {
	if(vector >= 0x20 && vector <= 0x27) 
		outb(PIC_MASTER_CMD,PIC_CMD_EOI);
    else if(vector >= 0x28 && vector <= 0x2f) {
		outb(PIC_SLAVE_CMD,PIC_CMD_EOI); 
		outb(PIC_MASTER_CMD,PIC_CMD_EOI);
	}
}
