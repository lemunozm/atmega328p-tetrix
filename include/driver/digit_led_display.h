#ifndef _SDK_DRIVER_DIGIT_LED_DISPLAY_H_
#define _SDK_DRIVER_DIGIT_LED_DISPLAY_H_

#include "spi.h"

#define DLD_OP_NOP  0x0

#define DLD_OP_DIGIT_0  0x1
#define DLD_OP_DIGIT_1  0x2
#define DLD_OP_DIGIT_2  0x3
#define DLD_OP_DIGIT_3  0x4
#define DLD_OP_DIGIT_4  0x5
#define DLD_OP_DIGIT_5  0x6
#define DLD_OP_DIGIT_6  0x7
#define DLD_OP_DIGIT_7  0x8

#define DLD_OP_DECODE_MODE  0x9
#define DLD_OP_INTENSITY    0xA
#define DLD_OP_SCAN_LIMIT   0xB
#define DLD_OP_SHUTDOWN     0xC
#define DLD_OP_DISPLAY_TEST 0xF

typedef struct DigitLedDisplay
{
    SPI* spi;
    uint8_t devices;

} DigitLedDisplay;

void init_dld(DigitLedDisplay* dld, SPI* spi, uint8_t devices);

void dld_write(DigitLedDisplay* dld, uint8_t opcode, uint8_t data);
void dld_write_values(DigitLedDisplay* dld, uint8_t opcode, uint8_t* data);

void dld_light(DigitLedDisplay* dld, bool value);
void dld_row(DigitLedDisplay* dld, uint8_t x, uint8_t* value);
void dld_clear(DigitLedDisplay* dld);
void dld_fill(DigitLedDisplay* dld);

#endif // _SDK_DRIVER_DIGIT_LED_DISPLAY_H_
