#include "driver/digit_led_display.h"

void init_dld(DigitLedDisplay* dld, SPI* spi, uint8_t devices)
{
    dld->spi = spi;
    dld->devices = devices;

    dld_write(dld, DLD_OP_DISPLAY_TEST, 0);
    dld_write(dld, DLD_OP_SCAN_LIMIT, 7);
    dld_write(dld, DLD_OP_DECODE_MODE, 0);
    dld_clear(dld);
    dld_light(dld, false);
}

void dld_write(DigitLedDisplay* dld, uint8_t opcode, uint8_t data)
{
    spi_enable(dld->spi, true);
    for(int i = 0; i < dld->devices; i++)
    {
        spi_write(dld->spi, opcode);
        spi_write(dld->spi, data);
    }
    spi_enable(dld->spi, false);
}

void dld_write_values(DigitLedDisplay* dld, uint8_t opcode, uint8_t* data)
{
    spi_enable(dld->spi, true);

    for(int i = 0; i < dld->devices; i++)
    {
        spi_write(dld->spi, opcode);
        spi_write(dld->spi, data[i]);
    }

    spi_enable(dld->spi, false);
}

void dld_light(DigitLedDisplay* dld, bool value)
{
    dld_write(dld, DLD_OP_SHUTDOWN, value);
}

void dld_row(DigitLedDisplay* dld, uint8_t x, uint8_t* values)
{
    dld_write_values(dld, DLD_OP_DIGIT_0 + x, values);
}

void dld_clear(DigitLedDisplay* dld)
{
    for(int i = 0; i < 8; i++)
    {
        dld_write(dld, DLD_OP_DIGIT_0 + i, 0);
    }
}

void dld_fill(DigitLedDisplay* dld)
{
    for(int i = 0; i < 8; i++)
    {
        dld_write(dld, DLD_OP_DIGIT_0 + i, 0B11111111);
    }
}

