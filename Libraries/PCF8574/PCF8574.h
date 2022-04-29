#ifndef PCF8574_H
#define PCF8574_H
#include "TWI.h"

/* ADDRESS TABLE
| INPUTS | PCF8574 | PCF8574A |
|   000  |   0x20  |   0x38   |
|   001  |   0x21  |   0x39   |
|   010  |   0x22  |   0x3A   |
|   011  |   0x23  |   0x3B   |
|   100  |   0x24  |   0x3C   |
|   101  |   0x25  |   0x3D   |
|   110  |   0x26  |   0x3E   |
|   111  |   0x27  |   0x3F   |
*/

void PCF8574_begin  (uint8_t address);
void PCF8574_write  (uint8_t address, uint8_t data);

void PCF8574_begin(uint8_t address)
{
	TWI_beginTransmission(address);
	TWI_writeData(0x00);
	TWI_endTransmission();
}

void PCF8574_write(uint8_t address, uint8_t data)
{
	TWI_beginTransmission(address);
	TWI_writeData(data);
	TWI_endTransmission();
}
#endif