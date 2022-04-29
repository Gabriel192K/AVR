#ifndef LCD_H
#define LCD_H
#include <avr/io.h>
#include <util/delay.h>

/********************************************************************************************************************
In work...
********************************************************************************************************************/
#define DATA_DIR DDRA
#define DATA_PORT PORTA
#define COMMAND_DIR DDRC
#define COMMAND_PORT PORTC
#define RS PORTC7
#define RW PORTC6
#define EN PORTC5

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

static struct LCD
{
	uint8_t _rows, _cols, _mode;
	uint8_t _displayFunction, _displayControl, _displayMode;
}_lcd;

void LCD_begin    (uint8_t cols, uint8_t rows, uint8_t mode);
void LCD_clear    (void);
void LCD_home     (void);
void LCD_setCursor(uint8_t cols, uint8_t rows);
void LCD_print    (char* s);
void LCD_printChar(char c);
void LCD_printInt (int n);
static inline void LCD_data(char data);
static inline void LCD_command(uint8_t _command);

void LCD_begin(uint8_t cols, uint8_t rows, uint8_t mode)
{
	// Set cols and rows
	_lcd._cols = cols; _lcd._rows = rows, _lcd._mode = mode;
	// Basic port initalization
	DATA_DIR	 = 0xFF;
	COMMAND_DIR |= (1 << RS) | (1 << RW) | (1 << EN);
	// Wait a bit
	_delay_ms(100);
	// Set lines number // todo: Implement 4 lines version too
	_lcd._displayFunction = rows > 1 ? _lcd._displayFunction | _2_LINE : _lcd._displayFunction | _1_LINE;
	// Set default LCD font and data mode
	_lcd._displayFunction |= _5x11_DOTS | _lcd._mode;
	LCD_command(FUNCTION_SET | _lcd._displayFunction);
	// Set default LCD display on, cursor off and blinck off
	_lcd._displayControl |= DISPLAY_ON | CURSOR_OFF | CURSOR_BLINK_OFF;
	LCD_command(DISPLAY_CONTROL | _lcd._displayControl);
	// Set default LCD mode
	_lcd._displayMode = DECREMENT_DDRAM | SHIFT_LEFT;
	LCD_command(ENTRY_MODE_SET | _lcd._displayMode);
	LCD_clear();
	LCD_home();
}

void LCD_clear()
{
	LCD_command(CLEAR_DISPLAY);
	_delay_ms(2);
}

void LCD_home()
{
	LCD_command(RETURN_HOME);
	_delay_ms(2);
}

void LCD_setCursor(uint8_t cols, uint8_t rows)
{
	const uint8_t rowOffsets[4] = {0x00, 0x40, 0x14, 0x54};
	rows = (rows >= _lcd._rows) ? _lcd._rows - 1 : rows;
	LCD_command(SET_DDRAM_ADDR | (cols + rowOffsets[rows]));
}

void LCD_print(char* s)
{
	for (uint8_t i = 0; s[i] != 0; i++)
		LCD_printChar(s[i]);
}

void LCD_printChar(char c)
{
	LCD_data(c);
}

static inline void LCD_data(char data)
{
	switch (_lcd._mode)
	{
		case _8_BIT_MODE:
			DATA_PORT = data;
			COMMAND_PORT |= (1 << RS);
			COMMAND_PORT &= ~(1 << RW);
			COMMAND_PORT |= (1 << EN);
			_delay_us(1);
			COMMAND_PORT &= ~(1 << EN);
			_delay_ms(1);
		break;
		case _4_BIT_MODE:
			DATA_PORT = data & 0xF0;
			COMMAND_PORT |= (1 << RS);
			COMMAND_PORT &= ~(1 << RW);
			COMMAND_PORT |= (1 << EN);
			_delay_us(1);
			COMMAND_PORT &= ~(1 << EN);
			_delay_ms(3);
			DATA_PORT = (data << 4) & 0xF0;
			COMMAND_PORT |= (1 << RS);
			COMMAND_PORT &= ~(1 << RW);
			COMMAND_PORT |= (1 << EN);
			_delay_us(1);
			COMMAND_PORT &= ~(1 << EN);
			_delay_ms(3);
		break;
		default: break;
	}
	
}

static inline void LCD_command(uint8_t command)
{
	switch (_lcd._mode)
	{
		case _8_BIT_MODE:
			DATA_PORT = command;
			COMMAND_PORT &= ~(1 << RS); 
			COMMAND_PORT &= ~(1 << RW);
			COMMAND_PORT |= (1 << EN);
			_delay_us(1);
			COMMAND_PORT &= ~(1 << EN);
			_delay_ms(1);
		break;
		case _4_BIT_MODE:
			DATA_PORT = command & 0xF0;
			COMMAND_PORT &= ~(1 << RS);
			COMMAND_PORT &= ~(1 << RW);
			COMMAND_PORT |= (1 << EN);
			_delay_us(1);
			COMMAND_PORT &= ~(1 << EN);
			_delay_ms(3);
			DATA_PORT = (command << 4) & 0xF0;
			COMMAND_PORT &= ~(1 << RS);
			COMMAND_PORT &= ~(1 << RW);
			COMMAND_PORT |= (1 << EN);
			_delay_us(1);
			COMMAND_PORT &= ~(1 << EN);
			_delay_ms(3);
		break;
		default: break;
	}
}

#endif