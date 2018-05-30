#ifndef _SDK_LFSR_H_
#define _SDK_LFSR_H_

#include <stdint.h>

#define LFSR_MAX 255

typedef struct LFSR
{
    uint16_t state;

} LFSR;

void init_lfsr(LFSR* lfsr, uint16_t seed);

uint8_t lfsr_generate(LFSR* lfsr);

#endif // _SDK_LFSR_H_
