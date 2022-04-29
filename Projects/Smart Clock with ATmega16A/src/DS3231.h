#ifndef DS3231_H
#define DS3231_H
#include "TWI.h"

// DS3231 address
#define DS3231_ADDRESS ((uint8_t)0x68)

// DS3231 registers
// ******************************************************************************************************************
#define DS3231_SECOND_REGISTER    ((uint8_t)0x00)
#define DS3231_MINUTE_REGISTER    ((uint8_t)0x01)
#define DS3231_HOUR_REGISTER      ((uint8_t)0x02)
#define DS3231_DAY_REGISTER       ((uint8_t)0x03)
#define DS3231_DATE_REGISTER      ((uint8_t)0x04)
#define DS3231_MONTH_REGISTER     ((uint8_t)0x05)
#define DS3231_YEAR_REGISTER      ((uint8_t)0x06)
#define DS3231_STATUS_REGISTER    ((uint8_t)0x0F)
#define DS3231_TEMP_HIGH_REGISTER ((uint8_t)0x11)
#define DS3231_TEMP_LOW_REGISTER  ((uint8_t)0x12)

// DS3231 register bits
// ******************************************************************************************************************
#define DS3231_STOP_CLOCK_BIT       ((uint8_t)0x80)
#define DS3231_TEMPERATURE_SIGN_BIT ((uint8_t)0x80)
#define DS3231_OSCILLATOR_STOP_BIT  ((uint8_t)0x80)

/*********************************************
Function prototypes
*********************************************/
void    DS3231_begin         (void);
void    DS3231_setTime       (uint8_t hour, uint8_t minute, uint8_t second);
void    DS3231_getTime       (uint8_t* hour, uint8_t* minute, uint8_t* second);
void    DS3231_setDate       (uint8_t date, uint8_t month, uint8_t year);
void    DS3231_getDate       (uint8_t* date, uint8_t* month, uint8_t* year);
void    DS3231_setDayofWeek  (uint8_t dayOfWeek);
void    DS3231_getDayOfWeek  (uint8_t* dayOfWeek);
void    DS3231_getTemperature(int8_t* temperature);
uint8_t DS3231_powerDown     (void);
static uint8_t bcdToDec(uint8_t bcd);
static uint8_t decToBcd(uint8_t dec);

/*********************************************
Function: begin
Purpose:  None
Input:    None
Return:   None
*********************************************/
void DS3231_begin(void)
{
}

/*********************************************
Function: setTime()
Purpose:  Set time
Input:    Hour, minute and second values
Return:   None
*********************************************/
void DS3231_setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
	TWI_beginTransmission(DS3231_ADDRESS); // Begin transmission to DS3231
	TWI_write(DS3231_SECOND_REGISTER);     // Start with second register
	TWI_write(DS3231_STOP_CLOCK_BIT);      // Set the stop clock bit
	TWI_write(decToBcd(minute));           // Write minute
	TWI_write(decToBcd(hour));             // Write hour
	TWI_endTransmission();                 // End transmission to DS3231
	TWI_beginTransmission(DS3231_ADDRESS); // Begin transmission to DS3231
	TWI_write(DS3231_SECOND_REGISTER);     // Start with second register
	TWI_write(decToBcd(second));           // Write second
	TWI_endTransmission();                 // End transmission to DS3231
}

/*********************************************
Function: getTime()
Purpose:  Get values of hour, minute and second
Input:    Pointers to hour, minute and second
Return:   None
*********************************************/
void DS3231_getTime(uint8_t* hour, uint8_t* minute, uint8_t* second)
{
	TWI_beginTransmission(DS3231_ADDRESS);                          // Begin transmission to DS3231
	TWI_write(DS3231_SECOND_REGISTER);                              // Start with second register
	TWI_requestFrom(DS3231_ADDRESS);                                // Request data from DS3231
	*second = bcdToDec(TWI_read(TWI_ACK) & ~DS3231_STOP_CLOCK_BIT); // Read second
	*minute = bcdToDec(TWI_read(TWI_ACK));                          // Read minute
	*hour   = bcdToDec(TWI_read(TWI_NACK));                         // Read hour
	TWI_endTransmission();                                          // End transmission to DS3231
}

/*********************************************
Function: setDate()
Purpose:  Set date
Input:    Date, month And year values
Return:   None
*********************************************/
void DS3231_setDate(uint8_t date, uint8_t month, uint8_t year)
{
	TWI_beginTransmission(DS3231_ADDRESS); // Begin transmission to DS3231
	TWI_write(DS3231_DATE_REGISTER);       // Start from date register
	TWI_write(decToBcd(date));             // Write date
	TWI_write(decToBcd(month));            // Write month
	TWI_write(decToBcd(year));             // Write year
	TWI_endTransmission();                 // End transmission to DS3231
}

/*********************************************
Function: getDate()
Purpose:  Get values of date, month and year
Input:    Pointers to date, month and year
Return:   None
*********************************************/
void DS3231_getDate(uint8_t* date, uint8_t* month, uint8_t* year)
{
	TWI_beginTransmission(DS3231_ADDRESS); // Begin transmission to DS3231
	TWI_write(DS3231_DATE_REGISTER);       // Start from date register
	TWI_requestFrom(DS3231_ADDRESS);       // Request data from DS3231
	*date = bcdToDec(TWI_read(TWI_ACK));   // Read date
	*month = bcdToDec(TWI_read(TWI_ACK));  // Read month
	*year = bcdToDec(TWI_read(TWI_NACK));  // Read year
	TWI_endTransmission();                 // End transmission to DS3231
}

/*********************************************
Function: setDayOfWeek()
Purpose:  Set day of week
Input:    day of week value
Return:   None
*********************************************/
void DS3231_setDayOfWeek(uint8_t dayOfWeek)
{
	TWI_beginTransmission(DS3231_ADDRESS); // Begin transmission to DS3231
	TWI_write(DS3231_DAY_REGISTER);        // Start with day register
	TWI_write(decToBcd(dayOfWeek));        // Write day of week
	TWI_endTransmission();                 // End transmission to DS3231
}

/*********************************************
Function: getDayOfWeek()
Purpose:  Get value of day of week
Input:    Pointer to day of week
Return:   None
*********************************************/
void DS3231_getDayOfWeek(uint8_t* dayOfWeek)
{
	TWI_beginTransmission(DS3231_ADDRESS);     // Begin transmission to DS3231
	TWI_write(DS3231_DAY_REGISTER);            // Start with day register
	TWI_requestFrom(DS3231_ADDRESS);           // Request data from DS3231
	*dayOfWeek = bcdToDec(TWI_read(TWI_NACK)); // Read day of week
	TWI_endTransmission();                     // End transmission to DS3231
}

/*********************************************
Function: getTemperature()
Purpose:  Get value of internal temperature sensor
Input:    Pointer to temperature
Return:   None
*********************************************/
void DS3231_getTemperature(int8_t* temperature)
{
	TWI_beginTransmission(DS3231_ADDRESS); // Begin transmission to DS3231
	TWI_write(DS3231_TEMP_HIGH_REGISTER);  // Start with temperature MSB register
	TWI_requestFrom(DS3231_ADDRESS);       // Request data from DS3231
	*temperature = TWI_read(TWI_NACK);     // Read temperature
	if (*temperature & DS3231_TEMPERATURE_SIGN_BIT) *temperature *= (int8_t)-1;
	TWI_endTransmission();
}

uint8_t DS3231_powerDown(void)
{
	TWI_beginTransmission(DS3231_ADDRESS);                    // Begin transmission to DS3231
	TWI_write(DS3231_STATUS_REGISTER);                        // Start with status register
	TWI_requestFrom(DS3231_ADDRESS);                          // Request data from DS3231
	return (TWI_read(TWI_NACK) & DS3231_OSCILLATOR_STOP_BIT); // Return 1 if power was down
}

/*********************************************
Function: bcdToDec()
Purpose:  Convert binary coded decimal to decimal
Input:    Value of bcd
Return:   Value of dec
*********************************************/
static uint8_t bcdToDec(uint8_t bcd)
{
	return (((bcd / (uint8_t)16) * (uint8_t)10) + (bcd % (uint8_t)16));
}

/*********************************************
Function: decToBcd()
Purpose:  Convert decimal to binary coded decimal
Input:    Value of dec
Return:   Value of bcd
*********************************************/
static uint8_t decToBcd(uint8_t dec)
{
	return (((dec / (uint8_t)10) * (uint8_t)16) + (dec % (uint8_t)10));
}
#endif
