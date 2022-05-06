#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

/********************************************************************************************************************
UDR - USART Data Register
	- It has two registers, one for TX and one for RX
	- Both TX & RX share the same UDR register
*********************************************************************************************************************
UBRR - USART Baud rate Register
	 - 16 bit register used for setting the baud rate
*******************************************Equations for calculating UBRR********************************************
1)	Asynchronous normal mode (U2X = 0) 
	UBRR = F_CPU / (16 * Baud Rate) - 1
	UBRR = (16000000 / 16 * 9600) - 1 = (16000000 / 153600) - 1 = 104.16 - 1 = 103.16
	UBRR = (16000000 / 16 * 115200) - 1 = (16000000 / 1843200) - 1 = 8
2)	Asynchronous double speed mode (U2X = 1)
	UBRR = F_CPU / (8 * Baud Rate) - 1
	UBRR = (16000000 / 8 * 9600) - 1 = (16000000 / 76800) - 1 = 208.33 - 1 = 207.33
	UBRR = (16000000 / 8 * 115200) - 1 = (16000000 / 921600) - 1 = 17.36 - 1 = 16.36
3)	Synchronous master mode
	UBRR = F_CPU / (2 * Baud Rate) - 1
	UBRR = (16000000 / 2 * 9600) - 1 = (16000000 / 19200) - 1 = 833.33 - 1 = 832.33
	UBRR = (16000000 / 2 * 115200) - 1 = (16000000 / 230400) - 1 = 69.44 - 1 = 68.44
*********************************************************************************************************************
UCSRA - USART Control and Status Register A
	  - Used for control and status flags
UCSRA BITMASK
| BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0 |
|  RXC  |  TXC  | UDRE  |  FE   |  DOR  |  PE   | U2X   | MPCM  |
RXC  - USART Receive Complete
	 - This flag bit is 1 when there is unread data in UDR
	 - Can be used to generate a Receive Complete Interrupt
TXC  - USART Transmit Complete
	 - This flag bit is 1 when the entire frame from TX buffer is shifted out and there is no new data in UDR
	 - Can be used to generate a Transmit Complete Interrupt
UDRE - USART Data Register Empty
	 - This flag bit is 1 when the buffer is empty which indicates UDR is ready to receive new data
	 - Can be used to generate a Data Register Empty Interrupt
	 - This flag bit is 1 after a reset to indicate transmitter is ready
FE   - Frame Error
DOR  - Data OverRun
	 - This flag bit is 1 if a data overrun condition is detected
	 - Data overrun occurs when the RX buffer is full (2 Byte) and a new byte is waiting in the RX Shift Register
PE   - Parity Error
U2X  - Double the USART Transmission Speed
MPCM - Multi Processor Communication Mode
*********************************************************************************************************************
UCSRB - USART Control and Status Register B
	  - Used for control and status flags
UCSRB BITMASK
| BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0 |
| RXCIE | TXCIE | UDRIE | RXEN  | TXEN  | UCSZ2 | RXB8  | TXB8  |
RXCIE - RX Complete Interrupt Enable
	  - Used to enable RX interrupts
TXCIE - TX Complete Interrupt Enable
	  - Used to enable TX interrupts
UDRIE - USART Data Register Empty Interrupt Enable
	  - Used to enable UDRE interrupts
RXEN  - RX Enable
	  - Used to enable RX
TXEN  - TX Enable
	  - Used to enable TX
UCSZ2 - USART Character Size
	  - Used to set character size
RXB8  - Receive Data Bit 8
TXB8  - Transmit Data Bit 8
*********************************************************************************************************************
UCSRC - USART Control and Status Register C
	  - Used for control and status flags
UCSRC BITMASK
| BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0 |
| URSEL | UMSEL | UPM1  | UPM0  | USBS  | UCSZ1 | UCSZ0 | UCPOL |
URSEL	   - USART Register Select
		   - Used to select between UCSRC (1) or UBRRH register (0)
UMSEL	   - USART Mode Select
		   - Used to select between A-sync (0) and Sync (1) type of operations
UPM1/UPM0  - USART Parity Mode
		   - Used to set type of parity generation and check
| UPM1 | UPM0 | Parity Mode |
|  0   |  0   |  Disabled   |
|  0   |  1   |  Reserved   |
|  1   |  0   | Even parity |
|  1   |  1   | Odd parity  |
USBS		- USART Stop Bit Select
			- Used to select 1 stop bit (0) or 2 stop bits (1)
UCSZ1/UCSZ0 - USART Character Size
			- Used to set character size
| UCSZ2 | UCSZ1 | UCSZ0 | Character size |
|   0   |   0   |   0   |     5 bit      |
|   0   |   0   |   1   |     6 bit      |
|   0   |   1   |   0   |     7 bit      |
|   0   |   1   |   1   |     8 bit      |
|   1   |   0   |   0   |    Reserved    |
|   1   |   0   |   1   |    Reserved    |
|   1   |   1   |   0   |    Reserved    |
|   1   |   1   |   1   |     9 bit      |
UCPOL - USART Clock Polarity
	  - Used to set clock polarity when using synchronous mode
	  - For safer behavior set this bit to 0
*********************************************************************************************************************
UBRRL/UBRRH - USART Baud Rate Registers
| BIT 15 | BIT 14 | BIT 13 | BIT 12 | BIT 11 | BIT 10 | BIT 9  | BIT 8  |
| URSEL  |   -    |   -    |   -    |            UBRRH[11:8]            |
| BIT 7  | BIT 6  | BIT 5  | BIT 4  | BIT 3  | BIT 2  | BIT 1  | BIT 0  |
|                              UBRRL[7:0]                               |
********************************************************************************************************************/

#define UART_RX_BUFFER_SIZE 128 // 128 bytes size
#define UART_TX_BUFFER_SIZE	128	// 128 bytes size

#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1) // Used to mask received data within 0 and (buffer size - 1)
#define UART_TX_BUFFER_MASK (UART_TX_BUFFER_SIZE - 1) // Used to mask transmitted data within 0 and (buffer size - 1)

#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
	#error "RX buffer size is not a power of 2"
#endif
#if (UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK)
	#error "TX buffer size is not a power of 2"
#endif

#if defined(__AVR_ATmega8__)  || defined(__AVR_ATmega8A__) \
 || defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) \
 || defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__)
	#define ATMEGA_USART
	#define UART0_RX_INTERRUPT USART_RXC_vect
	#define UART0_TX_INTERRUPT USART_UDRE_vect
	#define UART0_DATA		   UDR
	#define UART0_STATUS	   UCSRA
	#define UART0_CONTROL	   UCSRB
	#define UART0_UDRIE		   UDRIE
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) \
   || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) \
   || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__)\
   || defined(__AVR_ATmega328P__) 
	#define ATMEGA_USART0
	#define UART0_RX_INTERRUPT  USART_RX_vect
	#define UART0_TX_INTERRUPT  USART_UDRE_vect
	#define UART0_DATA			UDR0
	#define UART0_STATUS		UCSR0A
	#define UART0_CONTROL   	UCSR0B
	#define UART0_UDRIE			UDRIE0
#else
	#error "no UART definition for MCU available"
#endif

#if defined(ATMEGA_USART) || defined(ATMEGA_USART0)
	static volatile uint8_t UART_RX_BUFFER[UART_RX_BUFFER_SIZE];
	static volatile uint8_t UART_TX_BUFFER[UART_TX_BUFFER_SIZE];
	static volatile uint8_t UART_RX_HEAD, UART_RX_TAIL;
	static volatile uint8_t UART_TX_HEAD, UART_TX_TAIL;
#endif

#if defined(ATMEGA_USART) || defined(ATMEGA_USART0)

/*********************
Glossary of functions
*********************/
void     UART_begin	   (uint32_t _baudrate);
uint16_t UART_available(void);
void     UART_flush	   (void);
void     UART_printf   (char* format, ...);
static void print(const char* s);
static void send (const char c);

//uint8_t		   UART_Read_Char  (void);
//uint8_t		   UART_Read_String(char* _string);

/************************
Function: Interrupt Service Routines
Purpose:  Handling interrupts of UART0
Input:    Interrupt vector
Return:   None
************************/
ISR (UART0_RX_INTERRUPT)
{
	uint8_t _tempHead;
	uint8_t _data;
	
	#if defined(ATMEGA_USART) || defined(ATMEGA_USART0)
	_data = UART0_DATA;
	#endif
	// Calculate buffer index
	_tempHead = (UART_RX_HEAD + 1) & UART_RX_BUFFER_MASK;
	
	UART_RX_HEAD = _tempHead;
	UART_RX_BUFFER[_tempHead] = _data;
}
ISR (UART0_TX_INTERRUPT)
{
	uint8_t _tempTail;
	
	if (UART_TX_HEAD != UART_TX_TAIL)
	{
		// Calculate buffer index
		_tempTail = (UART_TX_TAIL + 1) & UART_TX_BUFFER_MASK;
		UART_TX_TAIL = _tempTail;
		// Transmit data
		#if defined(ATMEGA_USART) || defined(ATMEGA_USART0)
		UART0_DATA = UART_TX_BUFFER[_tempTail];
		#endif
	}
	else
	{
		#if defined(ATMEGA_USART) || defined(ATMEGA_USART0)
		UART0_CONTROL &= ~(1 << UART0_UDRIE); // Disable UDRE interrupts
		#endif
	}
}

/*****************************************
Function: begin()
Purpose:  Initialize UART and set baud rate
Input:    Baud rate: 9600, 115200, etc.
Return:   None
*****************************************/
void UART_begin(uint32_t _baudrate)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UART_RX_HEAD = 0;
		UART_RX_TAIL = 0;
		UART_TX_HEAD = 0;
		UART_TX_TAIL = 0;
	}
	// Set prescaler
	uint16_t _prescale = (F_CPU / 16 / _baudrate) - 1;
	
	#if defined(ATMEGA_USART)
		UBRRH = (uint8_t)(_prescale >> 8);
		UBRRL = (uint8_t)_prescale;
		// Enable RX & TX with Interrupts
		UART0_CONTROL |= (1 << RXEN) | (1 << RXCIE) | (1 << TXEN);
		// Select UCSRC Register & use 8 bit size
		UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	#elif defined(ATMEGA_USART0)
		UBRR0H = (uint8_t)(_prescale >> 8);
		UBRR0L = (uint8_t)_prescale;
		// Enable RX & TX with Interrupts
		UART0_CONTROL |= (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);
		#if defined(URSEL0)
			UCSR0C |= (1 << URSEL0) | (1 << UCSZ01) | (1 << UCSZ00);
		#else
			UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
		#endif
	#endif
	// Enable global interrupts
	sei();
}

/***************************************************************
Function: available()
Purpose:  Get the number of bytes waiting in the receiver buffer
Input:    None
Return:   Number of bytes waiting in the receiver buffer
***************************************************************/
uint16_t UART_available(void)
{
	uint8_t _ret;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		_ret = (UART_RX_BUFFER_SIZE + UART_RX_HEAD - UART_RX_TAIL) & UART_RX_BUFFER_MASK;
	}
	return _ret;
}

/***************************************************
Function: flush()
Purpose:  Flush bytes waiting in the receiver buffer
Input:    None
Return:   None
***************************************************/
void UART0_flush(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UART_RX_HEAD = UART_RX_TAIL;
	}
}

/***************************************************
Function: printf()
Purpose:  Printf emulation for UART
Input:    Format, arguments, etc.
Return:   None
***************************************************/
void UART_printf(char* format, ...)
{
	char buffer[UART_RX_BUFFER_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, UART_RX_BUFFER_SIZE, format, args);
	va_end(args);
	print(buffer);
}

/***************************************************
Function: print()
Purpose:  Static handler for printf
Input:    Pointer to char
Return:   None
***************************************************/
static void print(const char* s)
{
	while (*s)
		send(*s++);
}

/***************************************************
Function: send()
Purpose:  Static handler for print
Input:    Char to be sent
Return:   None
***************************************************/
static void send(const char c)
{
	uint8_t _tempHead;
	_tempHead = (UART_TX_HEAD + 1) & UART_TX_BUFFER_MASK;
	// Wait for free space in buffer
	while (_tempHead == UART_TX_TAIL);
	
	UART_TX_BUFFER[_tempHead] = c;
	UART_TX_HEAD = _tempHead;

	// Enable UDRE interrupts
	#if defined(ATMEGA_USART) || defined(ATMEGA_USART0)
	UART0_CONTROL |= (1 << UART0_UDRIE);
	#endif
}

/************************************
Function: UART0_Read_Char
Purpose:  Read char from ring buffer
Input:    None
Return:   Data to be read
************************************/
/*
uint8_t UART0_Read_Char(void)
{
	uint8_t _tempTail;
	uint8_t _data;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		if (UART_RX_HEAD == UART_RX_TAIL) 
			return 0; // might just return null char??
	}
	_tempTail = (UART_RX_TAIL + 1) & UART_RX_BUFFER_MASK;

	UART_RX_TAIL = _tempTail;
	_data = UART_RX_BUFFER[_tempTail];
	return _data;
}*/

/****************************************
Function: UART0_Read_String
Purpose:  Read string from ring buffer
Input:    String that holds the read data
Return:   State of data that's read
****************************************/
/*
uint8_t UART0_Read_String(char* _string)
{
	while (UART_available() > 0)
	{
		char _byte = UART0_Read_Char();
		if (_byte != '>') _strncat(_string, &_byte, 1);
		else return 1;
	}
	return 0;
}*/

/***************
Inline functions
***************/

/*
static char* _strncat(char* _destination, const char* _source, size_t _n)
{
	char* _p = _destination + _strlen(_destination); // Point to the end of the destination string
	while (* _source != '0' && _n--)				// Concatenate the char into the string
		*_p++ = *_source++;
	*_p = '\0';										// Null terminate the pointer
	return _destination;                            // Return the destination string
}

static uint8_t _strlen(char* _string)
{
	uint8_t _len = 0;                      // Make sure length is 0 by default
	for (; _string[_len] != '\0'; _len++); // While not a null char increase the length
	return _len;                           // Return the length
}*/

#endif
#endif