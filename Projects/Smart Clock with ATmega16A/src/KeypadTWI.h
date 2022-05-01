#ifndef KEYPADTWI_H
#define KEYPADTWI_H
#include "PCF8574.h"
#include "String.h"

struct
{
	uint8_t address, cols, rows;
	uint8_t index; char key, lastKey;
	unsigned long lastTime;
}_keypadTWI;

char keys[] = "123A456B789C*0#D";

void    KEYPADTWI_begin       (uint8_t address, uint8_t cols, uint8_t rows);
uint8_t KEYPADTWI_keyIsPressed(void);
char    KEYPADTWI_getKey      (void);
static uint8_t read(uint8_t mask);
static void    getIndex(void);
static uint8_t compareKey(void);


void KEYPADTWI_begin(uint8_t address, uint8_t cols, uint8_t rows)
{
	_keypadTWI.address = address; _keypadTWI.cols = cols; _keypadTWI.rows = rows;
	PCF8574_begin(_keypadTWI.address);
}

uint8_t KEYPADTWI_keyIsPressed(void)
{
	uint8_t buffer = read(0xF0);
	if (buffer == 0xFF) return 0;
	return (buffer != 0xF0);
}

char KEYPADTWI_getKey(void)
{
	getIndex();
	if (KEYPADTWI_keyIsPressed()) _keypadTWI.key = keys[_keypadTWI.index];
	if (millis() - _keypadTWI.lastTime >= 100UL)
	{
		_keypadTWI.lastTime = millis();
		if (compareKey())
			return _keypadTWI.key;
	}
	return '\0';
}

static uint8_t read(uint8_t mask)
{
	PCF8574_write(_keypadTWI.address, mask);
	return PCF8574_read(_keypadTWI.address);;
}

static void getIndex(void)
{
	uint8_t MSB = read(0xF0);
	uint8_t LSB = read(0x0F);
	for (uint8_t i = 0; i < 4; i++)
	{
		if (!((MSB >> (i + 4)) & 1))
		{
			_keypadTWI.index = i;
			break;
		}
	}
	for (uint8_t i = 0; i < 4; i++)
	{
		if (!((LSB >> i) & 1))
		{
			_keypadTWI.index += i * 4;
			break;
		}
	}
}

static uint8_t compareKey(void)
{
	if (_keypadTWI.key != _keypadTWI.lastKey)
	{
		_keypadTWI.lastKey = _keypadTWI.key;
		return 1;
	}
	return 0;
}

#endif
