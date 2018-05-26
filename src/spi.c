#include "spi.h"

void init_spi(SPI* spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ss, uint8_t bit_order)
{
    spi->mosi = mosi;
    spi->miso = miso;
    spi->sck = sck;
    spi->ss = ss;
    spi->bit_order = bit_order;

    if(mosi < UNUSED_PIN)
    {
        pin_mode(spi->mosi, PIN_OUTPUT);
    }

    if(miso < UNUSED_PIN)
    {
        pin_mode(spi->miso, PIN_INPUT);
    }

    pin_mode(spi->ss, PIN_OUTPUT);
    digital_pin_out(spi->ss, HIGH);

    pin_mode(spi->sck, PIN_OUTPUT);
    digital_pin_out(spi->sck, LOW);
}

void spi_enable(SPI* spi, bool value)
{
    digital_pin_out(spi->ss, !value);
}

void spi_write(SPI* spi, uint8_t value)
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t index = (spi->bit_order == SPI_LSB) ? i : 7 - i;
        digital_pin_out(spi->mosi, !!(value & (1 << index)));
        digital_pin_out(spi->sck, HIGH);
        digital_pin_out(spi->sck, LOW);
    }
}

uint8_t spi_read(SPI* spi)
{
    uint8_t value = 0;
    for (int i = 0; i < 8; i++)
    {
        digital_pin_out(spi->sck, HIGH);
        digital_pin_out(spi->sck, LOW);
        uint8_t index = (spi->bit_order == SPI_LSB) ? i : 7 - i;
        value |= digital_pin_in(spi->miso) << index;
    }
    return value;
}

