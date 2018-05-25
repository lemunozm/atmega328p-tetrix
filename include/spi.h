#ifndef _SDK_SPI_H_
#define _SDK_SPI_H_

#include "pin.h"

typedef struct SPIConnection
{
    uint8_t mosi;
    uint8_t miso;
    uint8_t sck;
    uint8_t ss;

}SPIConnection;

void init_spi_connection(SPIConnection* spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ss);

void spi_send(SPIConnection* spi, uint8_t opcode, int8_t data);
void spi_receive(SPIConnection* spi, uint8_t* opcode, int8_t* data, int poll);

#endif // _SDK_SPI_H_
