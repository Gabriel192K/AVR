#ifndef LCDTWI_H
#define LCDTWI_H
#include <stdarg.h>
#include <stdio.h>
#include "PCF8574.h"

// LCD commands
// ******************************************************************************************************************
#define CLEAR_DISPLAY   0x01
#define RETURN_HOME     0x02
#define ENTRY_MODE_SET  0x04
#define DISPLAY_CONTROL 0x08
#define CURSOR_SHIFT    0x10
#define FUNCTION_SET    0x20
#define SET_CGRAM_ADDR  0x40
#define SET_DDRAM_ADDR  0x80

// Flags for LCD entry mode set
// ******************************************************************************************************************

#define INCREMENT_DDRAM 0x01
#define DECREMENT_DDRAM 0x00
#define ENTRY_LEFT      0x02
#define ENTRY_RIGHT     0x00

// Flags for LCD on/off and cursor control
// ******************************************************************************************************************
#define DISPLAY_ON       0x04
#define DISPLAY_OFF      0x00
#define CURSOR_ON        0x02
#define CURSOR_OFF       0x00
#define CURSOR_BLINK_ON  0x01
#define CURSOR_BLINK_OFF 0x00

// Flags for LCD display and cursor shift
// ******************************************************************************************************************
#define SHIFT_DISPLAY 0x08
#define SHIFT_CURSOR  0x00
#define SHIFT_LEFT    0x00
#define SHIFT_RIGHT   0x04

// Flags for LCD function set
// ******************************************************************************************************************
#define _8_BIT_MODE 0x10
#define _4_BIT_MODE 0x00
#define _2_LINE     0x08
#define _1_LINE     0x00
#define _5x11_DOTS  0x04
#define _5x8_DOTS   0x00

// LCD macros
// ******************************************************************************************************************
#define LCD_RS (1 << 0)
#define LCD_RW (1 << 1)
#define LCD_EN (1 << 2)
#define LCD_BKL (1 << 3)
#define LCD_BSY (1 << 7)

/*********************************************
LCD struct
*********************************************/
static struct LCDTWI
{
	uint8_t buffer;
	uint8_t address, cols, rows;
	uint8_t displayFunction, displayControl, displayMode, backlight;
}_lcdTWI;

/*********************************************
Function prototypes
*********************************************/
void LCDTWI_begin      (uint8_t address, uint8_t cols, uint8_t rows);
void LCDTWI_home       (void);
void LCDTWI_clear      (void);
void LCDTWI_backlight  (void);
void LCDTWI_noBacklight(void);
void LCDTWI_setCursor  (uint8_t cols, uint8_t rows);
void LCDTWI_printf     (char* format, ...);
static void print  (char* s);
static void command(uint8_t command);
static void data   (char data);

/*********************************************
Function: begin()
Purpose:  Initialize LCD
Input:    Address of PCF8574 used, number of columns, number or rows
Return:   None
*********************************************/
void LCDTWI_begin(uint8_t address, uint8_t cols, uint8_t rows)
{
	_lcdTWI.address= address; _lcdTWI.cols = cols - 1; _lcdTWI.rows = rows - 1;
	_delay_ms(15);
	PCF8574_begin(_lcdTWI.address);
	_delay_ms(4.1); command(0x03); _delay_ms(4.1); command(0x03); _delay_ms(4.1); command(0x03); _delay_ms(4.1); command(0x02);
	_lcdTWI.displayFunction = (rows == 1) ? (_lcdTWI.displayFunction | _1_LINE) : (_lcdTWI.displayFunction | _2_LINE);
	_lcdTWI.displayFunction |= _5x8_DOTS | _4_BIT_MODE;                    command(FUNCTION_SET    | _lcdTWI.displayFunction);
	_lcdTWI.displayControl  |= DISPLAY_ON | CURSOR_OFF | CURSOR_BLINK_OFF; command(DISPLAY_CONTROL | _lcdTWI.displayControl);
	_lcdTWI.displayMode     |= DECREMENT_DDRAM | SHIFT_LEFT;               command(ENTRY_MODE_SET  | _lcdTWI.displayMode);
	LCDTWI_clear();
	LCDTWI_home();
	LCDTWI_backlight();
}

/*********************************************
Function: home()
Purpose:  Return cursor home (0, 0)
Input:    None
Return:   None
*********************************************/
void LCDTWI_home(void)
{
	command(RETURN_HOME);
}

/*********************************************
Function: clear()
Purpose:  Clear the LCD screen
Input:    None
Return:   None
*********************************************/
void LCDTWI_clear(void)
{
	command(CLEAR_DISPLAY);
}

/*********************************************
Function: backlight()
Purpose:  Enable the backlight
Input:    None
Return:   None
*********************************************/
void LCDTWI_backlight(void)
{
	if (!_lcdTWI.backlight)                             // Only if backlight is already off
	{
		_lcdTWI.backlight |= LCD_BKL;                   // Set backlignt
		_lcdTWI.buffer |= _lcdTWI.backlight;            // Copy backlight into buffer
		PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Update buffer
	}
}

/*********************************************
Function: noBacklight()
Purpose:  Disable backlight
Input:    None
Return:   None
*********************************************/
void LCDTWI_noBacklight(void)
{
	if (_lcdTWI.backlight)                              // Only if backlight is already on
	{
		_lcdTWI.backlight &= ~LCD_BKL;                  // Clear backlight
		_lcdTWI.buffer &= ~_lcdTWI.backlight;           // Copy backlight into buffer
		PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Update buffer
	}
}

/*********************************************
Function: setCursor()
Purpose:  Set the cursor position
Input:    Column position, row position
Return:   None
*********************************************/
void LCDTWI_setCursor(uint8_t cols, uint8_t rows)
{
	const uint8_t rowOffsets[4] = {0x00, 0x40, 0x14, 0x54};
	cols = (cols > _lcdTWI.cols) ? _lcdTWI.cols : cols;
	rows = (rows > _lcdTWI.rows) ? _lcdTWI.rows : rows;
	command(SET_DDRAM_ADDR | (cols + rowOffsets[rows]));
}

/*********************************************
Function: printf()
Purpose:  Printf a char array onto LCD
Input:    format, arguments
Return:   None
*********************************************/
void LCDTWI_printf(char* format, ...)
{
	char buffer[_lcdTWI.cols + 1];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, _lcdTWI.cols + 1, format, args);
	va_end(args);
	print(buffer);
}

/*********************************************
Function: print()
Purpose:  Print a char array onto LCD
Input:    Char array
Return:   None
*********************************************/
static void print(char* s)
{
	while (*s)
		data(*s++);
}

/*********************************************
Function: command()
Purpose:  Send command to LCD
Input:    command
Return:   None
*********************************************/
static void command(uint8_t command)
{
	// Top nibble
	_lcdTWI.buffer = command & 0xF0;                // Copy data into buffer
	_lcdTWI.buffer |= _lcdTWI.backlight;            // Check for backlight
	_lcdTWI.buffer &= ~LCD_RS;                      // Clear RS
	_lcdTWI.buffer &= ~LCD_RW;                      // Clear RW
	_lcdTWI.buffer |= LCD_EN;                       // Set EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_us(1);
	_lcdTWI.buffer &= ~LCD_EN;                      // Clear EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_ms(3);
	// Bottom nibble
	_lcdTWI.buffer = (command << 4) & 0xF0;         // Copy data into buffer
	_lcdTWI.buffer |= _lcdTWI.backlight;            // Check for backlight
	_lcdTWI.buffer &= ~LCD_RS;                      // Clear RS
	_lcdTWI.buffer &= ~LCD_RW;                      // Clear RW
	_lcdTWI.buffer |= LCD_EN;                       // Set EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_us(1);
	_lcdTWI.buffer &= ~LCD_EN;                      // Clear EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_ms(3);
}

/*********************************************
Function: data()
Purpose:  Send data to LCD
Input:    data
Return:   None
*********************************************/
static void data(char data)
{
	// Top nibble
	_lcdTWI.buffer = data & 0xF0;                   // Copy data into buffer
	_lcdTWI.buffer |= _lcdTWI.backlight;            // Check for backlight
	_lcdTWI.buffer |= LCD_RS;                       // Set RS
	_lcdTWI.buffer &= ~LCD_RW;                      // Clear RW
	_lcdTWI.buffer |= LCD_EN;                       // Set EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_us(1);
	_lcdTWI.buffer &= ~LCD_EN;                      // Clear EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_ms(3);
	// Lower nibble
	_lcdTWI.buffer = (data << 4) & 0xF0;            // Copy data into buffer
	_lcdTWI.buffer |= _lcdTWI.backlight;            // Check for backlight
	_lcdTWI.buffer |= LCD_RS;                       // Set RS
	_lcdTWI.buffer &= ~LCD_RW;                      // Clear RW
	_lcdTWI.buffer |= LCD_EN;                       // Set EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_us(1);
	_lcdTWI.buffer &= ~LCD_EN;                      // Clear EN
	PCF8574_write(_lcdTWI.address, _lcdTWI.buffer); // Send the buffer
	_delay_ms(3);
}
#endif