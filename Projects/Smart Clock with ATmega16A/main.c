/*
 * Smart Clock with ATmega16A.c
 *
 * Created: 4/11/2022 3:33:14 PM
 * Author : Rotes Gabriel
 */
#define F_CPU 16000000UL
#define LCD_Address 0x38
#define KEYPAD_Address 0x39
#include <stdio.h>
#include <string.h>
#include "src/TWI.h"
#include "src/Timers.h"
#include "src/LCDTWI.h"
#include "src/DS3231.h"
#include "src/AT24C32.h"
#include "src/KeypadTWI.h"
#include "src/UART.h"

struct
{
	uint8_t second, minute, hour, dayOfWeek, day, month, year;
	int8_t temperature;
}rtc;

uint8_t rd, wr;

char key;
char buffer[10];
uint8_t bufferLength;

unsigned long currentTime;
unsigned long lastSecond = 1000UL , lastTenSeconds = 10000UL;

void printData(void);
uint8_t handlePassword(void);

int main(void)
{
	UART0_Begin(9600);
	TIMER1_begin(1);
	TWI_begin(F_TWI_100K);
	LCDTWI_begin(LCD_Address, 20, 4);
	LCDTWI_printf("LCD 20x4 I2C");
	_delay_ms(1000);
	LCDTWI_clear();
	DS3231_begin();
	KEYPADTWI_begin(KEYPAD_Address, 4, 4);
	UART0_Send_String("Hello MF!");
	while (1) 
	{
		currentTime = millis();
		printData();
		while(handlePassword());
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
		UART0_Send_String("Hello MF!");
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
		LCDTWI_printf("%s", daysOfWeek[rtc.dayOfWeek - 1]);
		uint8_t ok = AT24C32_write(0x00, 0x9);
		rd = AT24C32_read(0x00);
		LCDTWI_setCursor(15, 0); LCDTWI_printf("0x%X %d", rd, ok);
	}
	
}



uint8_t handlePassword(void)
{
	const char password[] = "130802";
	key = KEYPADTWI_getKey();
	
	if (key)
	{
		if (bufferLength == 0) LCDTWI_clear();
		mystrcat(buffer, &key, 1);
		LCDTWI_setCursor(bufferLength, 0); LCDTWI_printf("%c", key);
		bufferLength++;
	}
	if (bufferLength == 6)
	{
		LCDTWI_setCursor(0, 0);
		if (mystrcmp(buffer, password, sizeof password) == 0)
			 LCDTWI_printf("Access Granted!");
		else
			LCDTWI_printf("Access Denied!");
		mymemset(buffer, 0);
		bufferLength = 0;
		currentTime = millis();
		while (millis() - currentTime <= 2000UL);
		lastSecond = 2000UL; lastTenSeconds = 11000UL;
		LCDTWI_clear();
	}
	return bufferLength;
}
