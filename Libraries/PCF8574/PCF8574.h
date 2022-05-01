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

/*********************************************
Function prototypes
*********************************************/
void PCF8574_begin(uint8_t address);
void PCF8574_write(uint8_t address, uint8_t data);
uint8_t PCF8574_read (uint8_t address);

/*********************************************
Function: begin()
Purpose:  Initialize PCF8574 with all pins at 0 volts
Input:    Address of PCF8574
Return:   None
*********************************************/
void PCF8574_begin(uint8_t address)
{
	TWI_beginTransmission(address);
	TWI_write(0x00);
	TWI_endTransmission();
}

/*********************************************
Function: write()
Purpose:  Write data to PCF8574
Input:    Address of PCF8574 and desired data
Return:   None
*********************************************/
void PCF8574_write(uint8_t address, uint8_t data)
{
	TWI_beginTransmission(address);
	TWI_write(data);
	TWI_endTransmission();
}

/*********************************************
Function: read()
Purpose:  Read data from PCF8574
Input:    Address of PCF8574
Return:   Data from PCF8574
*********************************************/
uint8_t PCF8574_read(uint8_t address)
{
	TWI_beginTransmission(address);
	TWI_requestFrom(address, 1);
	return TWI_read();
}
#endif