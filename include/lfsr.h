#ifndef _SDK_LFSR_H_
#define _SDK_LFSR_H_

#include <stdint.h>

#define LFSR_MAX 65535

typedef struct LFSR
{
    uint16_t state;

} LFSR;

void init_lfsr(LFSR* lfsr, uint16_t seed);

uint16_t lfsr_generate(LFSR* lfsr);
uint16_t lfsr_generate_bounded(LFSR* lfsr, uint16_t max);

#endif // _SDK_LFSR_H_
