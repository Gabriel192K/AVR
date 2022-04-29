/*
 * Smart Clock with ATmega16A.c
 *
 * Created: 4/11/2022 3:33:14 PM
 * Author : Rotes Gabriel
 */ 
#define F_CPU 16000000UL
#define LCD_Address 0x38
#include <stdio.h>
#include "src/TWI.h"
#include "src/Timers.h"
#include "src/LCDTWI.h"
#include "src/DS3231.h"
#include "src/AT24C32.h"

struct
{
	uint8_t second, minute, hour, dayOfWeek, day, month, year;
	int8_t temperature;
}rtc;

uint8_t rd, wr;

unsigned long currentTime;
unsigned long lastSecond = 1000UL , lastTenSeconds = 10000UL;

void printData(void);

int main(void)
{
	TIMER1_begin(1);
	TWI_begin(F_TWI_100K);
	LCDTWI_begin(LCD_Address, 20, 4);
	LCDTWI_printf("LCD 20x4 I2C");
	_delay_ms(1000);
	LCDTWI_clear();
	DS3231_begin();
	while (1) 
	{
		// lol
		currentTime = millis();
		printData();
	}
}

void printData(void)
{
	DS3231_getTime(&rtc.hour, &rtc.minute, &rtc.second);
	DS3231_getDate(&rtc.day, &rtc.month, &rtc.year);
	DS3231_getDayOfWeek(&rtc.dayOfWeek);
	DS3231_getTemperature(&rtc.temperature);
	if (currentTime - lastSecond > 1000UL)
	{
		lastSecond = currentTime;
		LCDTWI_setCursor(0, 0); LCDTWI_printf("Time: ");
		(rtc.hour < 10) ? LCDTWI_printf("0") : 0; LCDTWI_printf("%d:", rtc.hour);
		(rtc.minute < 10) ? LCDTWI_printf("0") : 0; LCDTWI_printf("%d:", rtc.minute);
		(rtc.second < 10) ? LCDTWI_printf("0") : 0; LCDTWI_printf("%d", rtc.second);
		LCDTWI_setCursor(0, 3); LCDTWI_printf("Temperature: ");
		LCDTWI_printf("%dC", rtc.temperature);
	}
	if (currentTime - lastTenSeconds > 10000UL)
	{
		lastTenSeconds = currentTime;
		LCDTWI_setCursor(0, 1); LCDTWI_printf("Date: ");
		(rtc.day < 10) ? LCDTWI_printf("0") : 0; LCDTWI_printf("%d/", rtc.day);
		(rtc.month < 10) ? LCDTWI_printf("0") : 0; LCDTWI_printf("%d/", rtc.month);
		(rtc.year < 10) ? LCDTWI_printf("0") : 0; LCDTWI_printf("%d", rtc.year);
		LCDTWI_setCursor(0, 2); LCDTWI_printf("DoW: ");
		const char daysOfWeek[7][9] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
		LCDTWI_printf("%s", daysOfWeek[rtc.dayOfWeek]);
		AT24C32_read(0x00, &rd);
		LCDTWI_setCursor(15, 0); LCDTWI_printf("0x%X", rd);
	}
	
}