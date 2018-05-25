#include "spi.h"

void init_spi_connection(SPIConnection* spi, uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ss)
{
    spi->mosi = mosi;
    spi->miso = miso;
    spi->sck = sck;
    spi->ss = ss;

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

void spi_send(SPIConnection* spi, uint8_t opcode, int8_t data)
{
    digital_pin_out(spi->ss, LOW);
    shift_out(spi->mosi, spi->sck, MSB, opcode);
    shift_out(spi->mosi, spi->sck, MSB, data);
    digital_pin_out(spi->ss, HIGH);
}

void spi_receive(SPIConnection* spi, uint8_t* opcode, int8_t* data, int poll)
{
    (void) spi; (void) opcode; (void) data; (void) poll;
    //TODO
}
