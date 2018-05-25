#ifndef _SDK_PORT_H_
#define _SDK_PORT_H_

#include <stdint.h>

#define PORT_OUTPUT 0B11111111
#define PORT_INPUT 0B00000000

#define PORT_0 0 //Port D
#define PORT_1 1 //Port B

void port_mode(uint8_t port, uint8_t dir);

void digital_port_out(uint8_t port, uint8_t value);
uint8_t digital_port_in(uint8_t port);


#endif // _SDK_PORT_H_
