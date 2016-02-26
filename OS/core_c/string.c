#include <string.h>
#include <stdint.h>
#include <io.h>

char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void itoa(uint32_t i, uint8_t base, char* buf) {
	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	int pos = 0;
	char temp_buf[32];
	while (i != 0) {
		temp_buf[pos++] = bchars[i % base];
		i /= base;
	}
	int top = pos--;
	int opos;
	for (opos = 0; opos < top; pos--, opos++) {
		buf[opos] = temp_buf[pos];
	}
	buf[opos] = 0;
}

uint32_t strlen(const char* str) {
	uint32_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
}

uint32_t strlen_trim(const char* str) {
	uint32_t ret = strlen(str);
	while (str[ret-1] == ' ' && (ret != 0))
		ret--;
	return ret;
}

void byte_swap(uint8_t *data, int len) {
	int i;
	uint8_t temp;
	for(i = 0; i < len; i += 2) {
		temp = data[i];
		data[i] = data[i+1];
		data[i+1] = temp;
	}
}