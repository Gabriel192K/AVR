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
	UART0_Begin(19200);
    UART0_Send_Char('C');
    while (1) 
    {
		 UART0_Send_Char('C');
		_delay_ms(1000);
    }
}

