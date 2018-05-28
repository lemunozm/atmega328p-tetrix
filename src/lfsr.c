#include "lfsr.h"

void init_lfsr(LFSR* lfsr, uint16_t seed)
{
    lfsr->state = seed;
}

uint8_t lfsr_generate(LFSR* lfsr)
{
    unsigned lsb = lfsr->state & 1;
    lfsr->state >>= 1;
    lfsr->state ^= (-lsb) & 0xB400u;
    lfsr->state ^= 0xB400u;
    return lfsr->state;
}

