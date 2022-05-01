#ifndef AT24C32_H
#define AT24C32_H
#include "TWI.h"

// AT24C16 address
#define AT24C32_ADDRESS 0x50

/*********************************************
Function prototypes
*********************************************/
void    AT24C32_begin(void);
uint8_t AT24C32_write(uint8_t address, uint8_t data);
uint8_t AT24C32_read (uint8_t address);
void    AT24C32_readArray(uint8_t address, uint8_t* data , size_t size);

/*********************************************
Function: begin()
Purpose:  None
Input:    None
Return:   None
*********************************************/
void AT24C32_begin(void)
{
}

/*********************************************
Function: write()
Purpose:  Write byte of data to a specific address
Input:    Address and data
Return:   1 if successfull and 0 if not successfull (already same value in that register)
*********************************************/
uint8_t AT24C32_write(uint8_t address, uint8_t data)
{
	uint8_t readValue = AT24C32_read(address);
	if (data != readValue)
	{
		TWI_beginTransmission(AT24C32_ADDRESS);
		TWI_write(address >> 8);
		TWI_write(address);
		TWI_write(data);
		TWI_endTransmission();
		_delay_ms(5);
		return 1;
	}
	return 0;
}

/*********************************************
Function: read()
Purpose:  Read byte of data from a specific address
Input:    Address where reading is requested
Return:   Data
*********************************************/
uint8_t AT24C32_read(uint8_t address)
{
	TWI_beginTransmission(AT24C32_ADDRESS);
	TWI_write(address >> 8);
	TWI_write(address);
	TWI_requestFrom(AT24C32_ADDRESS, 1);
	return (TWI_read());
}

void AT24C32_readArray(uint8_t address, uint8_t* data, size_t size)
{
	while (size--)
	{
		*data++ = AT24C32_read(address++);
	}
}

#endif