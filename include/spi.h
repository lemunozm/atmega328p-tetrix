#ifndef _SDK_SPI_H_
#define _SDK_SPI_H_

#include "pin.h"
#include <stdbool.h>

#define SPI_MSB 1
#define SPI_LSB 0

typedef struct SPI
{
    uint8_t mosi;
    uint8_t miso;
    uint8_t sck;
    uint8_t ss;
    uint8_t bit_order;
    //TODO: add clock read mode

} SPI;

void init_spi(SPI* spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ss, uint8_t bit_order);
void spi_enable(SPI* spi, bool value);

void spi_write(SPI* spi, uint8_t value);
uint8_t spi_read(SPI* spi); //NOT_TESTED

#endif // _SDK_SPI_H_
