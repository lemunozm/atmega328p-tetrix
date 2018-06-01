#include "lfsr.h"

void init_lfsr(LFSR* lfsr, uint16_t seed)
{
    lfsr->state = seed;
}

uint16_t lfsr_generate(LFSR* lfsr)
{
    unsigned lsb = lfsr->state & 1;
    lfsr->state >>= 1;
    lfsr->state ^= (-lsb) & 0xB400u;
    lfsr->state ^= 0xB400u;
    return lfsr->state;
}

uint16_t lfsr_generate_bounded(LFSR* lfsr, uint16_t max)
{
    uint16_t value = lfsr_generate(lfsr);
    uint16_t result = value / (LFSR_MAX / max);
    return  (result == max) ? result - 1 : result;
}
