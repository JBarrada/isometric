#include <pit.h>
#include <stdint.h>
#include <io.h>
#include <pic.h>

uint32_t milli_sleep = 0;
int sleep_on = 0;

void pit_irq() {
	if (sleep_on)
		milli_sleep++;
}

void msleep(uint32_t millis) {
	sleep_on = 1;
	milli_sleep = 0;
	while (milli_sleep < millis) {}
	sleep_on = 0;
}

static inline void __pit_send_cmd(uint8_t cmd) {
	outb(PIT_REG_COMMAND, cmd);
}

static inline void __pit_send_data(uint16_t data, uint8_t counter) {
	uint8_t	port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 : ((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);
	outb (port, (uint8_t)data);
}

static inline uint8_t __pit_read_data(uint16_t counter) {
	uint8_t	port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 : ((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);
	return inb (port);
}

static void pit_start_counter(uint32_t freq, uint8_t counter, uint8_t mode) {
	if (freq==0)
		return;

	uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);

	// send operational command words
	uint8_t ocw = 0;
	ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
	ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
	__pit_send_cmd(ocw);

	// set frequency rate
	__pit_send_data(divisor & 0xff, 0);
	__pit_send_data((divisor >> 8) & 0xff, 0);
}

void pit_init() {
	pit_start_counter(1000, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
	irq_clear_mask(0);
}
