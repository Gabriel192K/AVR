#ifndef TWI_H
#define TWI_H
#include <avr/io.h>

/********************************************************************************************************************
Revision 1.0
ADDRESS: 0x68 (MPU 6050)
0x68 = 0b01101000
Slave write address: (Address << 1) | 0 = 0xD0
Slave read address: (Address << 1) | 1 = 0xD1
*********************************************************************************************************************
TWBR - TWI Bit Rate Register
	 - Used to generate SCL frequency while in master mode
*********************************************************************************************************************
TWCR - TWI Control Register
	 - Used to control events of all I2C communications
TWCR BITMASK
| BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0 |
| TWINT | TWEA  | TWSTA | TWSTO | TWWC  | TWEN  |   -   | TWIE  |
*********************************************************************************************************************/
#define TWI_WRITE 0
#define TWI_READ  1
#define TWI_FREQ_100K 72
#define TWI_FREQ_250K 24
#define TWI_FREQ_400K 12
#define NACK 0
#define ACK  1

/*********************************************
Function: begin()
Purpose:  Initialize TWI
Input:    None
Return:   None
*********************************************/
void TWI_begin()
{
	DDRC  &= ~(1 << PORTC1) | (1 << PORTC0);
	PORTC &= ~(1 << PORTC1) | (1 << PORTC0);
	TWBR = TWI_FREQ_400K;
}

/*********************************************
Function: beginTransmission()
Purpose:  Begin transmission of data
Input:    Address where data is transmitted
Return:   None
*********************************************/
void TWI_beginTransmission(uint8_t _address)
{
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	_address = ((_address << 1) | TWI_WRITE);
	TWDR = _address;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

/*********************************************
Function: writeData()
Purpose:  Write data on the TWI bus
Input:    Byte of data to be sent
Return:   None
*********************************************/
void TWI_writeData(uint8_t _data)
{
	TWDR = _data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

/*********************************************
Function: requesrFrom()
Purpose:  Request data from slave
Input:    Address of the slave
Return:   None
*********************************************/
void TWI_requestFrom(uint8_t _address)
{
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	_address = ((_address << 1) | TWI_READ);
	TWDR = _address;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

/*********************************************
Function: read()
Purpose:  Read data from slave
Input:    ACK state
Return:   Data from slave
*********************************************/
uint8_t TWI_read(uint8_t readACK)
{
	if (readACK == ACK)
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		while (!(TWCR & (1 << TWINT)));
	}
	else if (readACK == NACK)
	{
		TWCR = (1 << TWINT) | (1 << TWEN);
		while (!(TWCR & (1 << TWINT)));
	}
	uint8_t _data = TWDR;
	return _data;
}

/*********************************************
Function: endTransmission()
Purpose:  End transmission of data
Input:    None
Return:   None
*********************************************/
void TWI_endTransmission()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
#endif