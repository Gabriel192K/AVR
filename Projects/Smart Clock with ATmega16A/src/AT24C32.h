#ifndef AT24C32_H
#define AT24C32_H
#include "TWI.h"

// AT24C16 address
#define AT24C32_ADDRESS 0x50

/*********************************************
Function prototypes
*********************************************/
void AT24C32_begin(void);
void AT24C32_write(uint8_t address, uint8_t data);
void AT24C32_read(uint8_t address, uint8_t* data);

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
Return:   None
*********************************************/
void AT24C32_write(uint8_t address, uint8_t data)
{
    TWI_beginTransmission(AT24C32_ADDRESS);
    TWI_write(address >> 8);
    TWI_write(address);
    TWI_write(data);
    TWI_endTransmission();
    _delay_ms(5);
}

/*********************************************
Function: read()
Purpose:  Read byte of data from a specific address
Input:    Address and pointer to data
Return:   None
*********************************************/
void AT24C32_read(uint8_t address, uint8_t* data)
{
    TWI_beginTransmission(AT24C32_ADDRESS);
    TWI_write(address >> 8);
    TWI_write(address);
    TWI_requestFrom(AT24C32_ADDRESS);
    *data = TWI_read(TWI_NACK);
    TWI_endTransmission();
}

#endif