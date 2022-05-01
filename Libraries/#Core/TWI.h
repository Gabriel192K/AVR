#ifndef TWI_H
#define TWI_H
#include <avr/io.h>

/********************************************************************************************************************
Revision 1.1
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

#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__)
	#define TWI_DDR  DDRC
	#define TWI_PORT PORTC
	#define TWI_SCL  PORTC0
	#define TWI_SDA  PORTC1
#endif

#define TWI_START()     (TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA))
#define TWI_STOP()      (TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO))
#define TWI_WRITE()     (TWCR = (1<<TWINT)|(1<<TWEN))
#define TWI_READ(ACK)   (TWCR = (1<<TWINT)|(1<<TWEN)|(ACK<<TWEA))

#define F_TWI_100K 72
#define F_TWI_250K 24
#define F_TWI_400K 12

static int8_t _bytes;

/*********************************************
Function prototypes
*********************************************/
uint8_t TWI_begin(uint8_t speed);
void    TWI_beginTransmission(uint8_t address);
void    TWI_write(uint8_t data);
void    TWI_requestFrom(uint8_t address, uint8_t bytes);
uint8_t TWI_read();
void    TWI_endTransmission();
static uint8_t handleSpeed(uint8_t speed);

/*********************************************
Function: begin()
Purpose:  Initialize TWI
Input:    None
Return:   1 if successful or loop endlessly if unsuccesful
*********************************************/
uint8_t TWI_begin(uint8_t speed)
{
	DDRC  &= ~(1 << TWI_SDA) | (1 << TWI_SCL);
	PORTC &= ~(1 << TWI_SDA) | (1 << TWI_SCL);
	while (!handleSpeed(speed));
	return 1;
}

/*********************************************
Function: beginTransmission()
Purpose:  Begin transmission of data
Input:    Address where data is transmitted
Return:   None
*********************************************/
void TWI_beginTransmission(uint8_t address)
{
	TWI_START();
	while (!(TWCR & (1 << TWINT)));
	address = (address << 1);
	TWDR = address;
	TWI_WRITE();
	while (!(TWCR & (1 << TWINT)));
}

/*********************************************
Function: write() | (old writeData())
Purpose:  Write data on the TWI bus
Input:    Byte of data to be sent
Return:   None
*********************************************/
void TWI_write(uint8_t data)
{
	TWDR = data;
	TWI_WRITE();
	while (!(TWCR & (1 << TWINT)));
}

/*********************************************
Function: requesrFrom()
Purpose:  Request data from slave
Input:    Address of the slave and amount of bytes supposed to get
Return:   None
*********************************************/
void TWI_requestFrom(uint8_t address, uint8_t bytes)
{
	_bytes = bytes;
	TWI_START();
	while (!(TWCR & (1 << TWINT)));
	address = ((address << 1) | 1);
	TWDR = address;
	TWI_WRITE();
	while (!(TWCR & (1 << TWINT)));
}

/*********************************************
Function: read()
Purpose:  Read data from slave
Input:    None
Return:   Data from slave
*********************************************/
uint8_t TWI_read()
{
	(--_bytes > 0) ? TWI_READ(1) : ((_bytes == 0) ? TWI_READ(0) : 0);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

/*********************************************
Function: endTransmission()
Purpose:  End transmission of data
Input:    None
Return:   None
*********************************************/
void TWI_endTransmission()
{
	TWI_STOP();
	while(TWCR & (1<<TWSTO));
}

/*********************************************
Function: handleSpeed()
Purpose:  Handle TWI speed selection
Input:    Speed of TWI
Return:   Return 1 if successful and 0 if not
*********************************************/
static uint8_t handleSpeed(uint8_t speed)
{
	switch (speed)
	{
		case F_TWI_100K: TWBR = F_TWI_100K; return 1;
		case F_TWI_250K: TWBR = F_TWI_250K; return 1;
		case F_TWI_400K: TWBR = F_TWI_400K; return 1;
		default: return 0;
	}
}

#endif