#ifndef __STRING_H_
#define __STRING_H_

#include <stdint.h>

void itoa(uint32_t i, uint8_t base, char* buf);
uint32_t strlen(const char* str);
uint8_t strcompare(const char* str1, const char* str2);

void byte_swap(uint8_t *data, int len);

#endif