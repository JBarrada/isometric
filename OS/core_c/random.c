#include <stdint.h>
#include <random.h>


uint32_t rnd_seed=1337, y=12, z=23432, w=213432;

uint32_t xorshift128() {
    uint32_t t = rnd_seed;
    t ^= t << 11;
    t ^= t >> 8;
    rnd_seed = y; y = z; z = w;
    w ^= w >> 19;
    w ^= t;
    return w;
}

uint32_t random_number(uint32_t a, uint32_t b) {
	return (xorshift128()/(0xffffffff/(b-a))) + a;
}