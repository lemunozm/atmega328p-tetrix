#include "pin.h"
#include "spi.h"
#include "port.h"

#include <util/delay.h>

int main(void)
{

    SPIConnection spi;
    init_spi_connection(&spi, DIGITAL_13, UNUSED_PIN, DIGITAL_11, DIGITAL_10);

    while(1)
    {
        spi_send(&spi, 0xF, 0);
        _delay_ms(200);
        spi_send(&spi, 0xF, 0);
        _delay_ms(200);
        spi_send(&spi, 0xF, 0);
        _delay_ms(200);
        spi_send(&spi, 0xF, 0);
        _delay_ms(200);
        spi_send(&spi, 0xF, 1);
        _delay_ms(200);
        spi_send(&spi, 0xF, 1);
        _delay_ms(200);
        spi_send(&spi, 0xF, 1);
        _delay_ms(200);
        spi_send(&spi, 0xF, 1);
        _delay_ms(200);
    }
    //spi_send(&spi, 0xB, 7);
    //spi_send(&spi, 0x9, 0);
    //spi_send(&spi, 0x2, 0B01010101);

    /*for(int i = 0; i < 8; i++)
    {
        spi_send(&spi, , i + 1);
    }
    //spi_send(&spi, 0xC, 0);
*/

    port_mode(PORT_0, PORT_OUTPUT);
    digital_port_out(PORT_0, 0x9);
}
