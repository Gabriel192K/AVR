/*
 * RFID Test.c
 *
 * Created: 5/5/2022 5:14:59 PM
 * Author : Rotes Gabriel
 */ 

#define F_CPU 12288000UL
#include "src/UART.h"
#include <util/delay.h>

int main(void)
{
	UART_begin(9600);
    UART_printf("Hello, World!\n");
	DDRD |= (1 << DDRD4);
    while (1)
    {
		 UART_printf("Hello, World!\n");
		 PORTD ^= (1 << PORTD4);
		_delay_ms(1000);
    }
}

