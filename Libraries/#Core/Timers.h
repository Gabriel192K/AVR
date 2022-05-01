#ifndef TIMERS_H
#define TIMERS_H
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <avr/io.h>

/*
F_TIMER = F_CPU / PRESCALER
TICK = 1 / F_TIMER
TOTAL = TICK * MAX_VALUE_OF_TIMER
OVERFLOW_COUNT = PERIOD_TO_OVERFLOW * TICK

// EXAMPLE
F_TIMER = 16000000 Hz / 1024 = 15.625 Hz | 16000000 Hz / 8 = 2.000.000 Hz
TICK = 1 / 15.625 = 0.000064 s = 64 us   | 1 / 2.000.000 = 0.0000005 s = 0.5 us = 500 ns
TOTAL = 0.000064 s * 65536 = 4.194304 s  | 0.0000005 * 65536 = 0.032768 s
To overflow every 1 ms we need 15.625    | 0.001 / 0.0000005 = 2000
15.625 means 15 + 0.625 and is not 15 thousand 625
or just use
OVF (value when will overflow)= F_CPU / 1000 / PRESCALER

//
// TIMER REGISTERS
//

TCCR0  - Timer Counter Control Register 0  (8 bit)
TCCR1A - Timer Counter Control Register 1A (16 bit)
TCCR1B - Timer Counter Control Register 1B (16 bit)
TCCR2  - Timer Counter Control Register 2  (8 bit)

// 
// TIMER 0 
//

In ATmega 16 / 32, the Timer counts up until the value of 
	the TCNT0 (Timer/counter register) register becomes equal to the content of OCR0 (Compare register)
As soon as TCNT0 becomes equal to the OCR0, a compare match occurs,
	and then the timer will get cleared and the OCF0 flag will get set.
OCF0 flag is located in the TIFR register.

TCCR0 BITMASK
| BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0 |
| FOC0  | WGM00 | COM01 | COM00 | WGM01 | CS02  | CS01  | CS00  |

FOC0 - Force compare match
- Write only bit, which can be used while generating a wave
- Writing 1 to this bit will force the wave generator to act as if a compare match has occurred

WGM00/WGM01 - Waveform Generation Mode
| WGM00 | WGM01 |      Waveform Generation Mode     |
|   0   |   0   |               normal              |
|   0   |   1   | CTC (Clear Timer on Compare Match |
|   1   |   0   |       PWM (Phase Correct)         |
|   1   |   1   |              Fast PWM             |

COM00/COM01 - Compare Output Mode
| COM01 | COM00 |  Mode name   |            Compare Output Mode              |
|   0   |   0   | Disconnected | The normal port operation, OC0 disconnected |
|   0   |   1   |   Reserved   |                 Reserved                    |
|   1   |   0   | Non-inverted | Clear OC0 on compare match, set OC0 at TOP  |
|   1   |   1   |   Inverted   | Set OC0 on compare match, clear OC0 at TOP  |

CS02/CS01/CS00 - Clock Source 
| CS02 | CS01 | CS00 |     Clock Source   |
|  0   |  0   |  0   |     Clock Stopped  |
|  0   |  0   |  1   |     Clock          |
|  0   |  1   |  0   |     Clock/8        |
|  0   |  1   |  1   |     Clock/64       |
|  1   |  0   |  0   |     Clock/256      |
|  1   |  0   |  1   |     Clock/1024     |
|  1   |  1   |  0   | Clock/T0 (Falling) |
|  1   |  1   |  1   | Clock/T0 (Rising)  |
*/

volatile unsigned long _timer1Counter;

ISR (TIMER1_COMPA_vect)
{
	_timer1Counter++;
}

void TIMER1_begin(uint8_t milliseconds)
{
	// Clear Timer on Compare && Prescaler F_CPU / 1024
	TCCR1B |= (1 << WGM12) | (1 << CS10);
	// Set the value of overflowing
	OCR1A = (milliseconds / 1000.) / (1. / F_CPU);
	//OCR1A = (F_CPU / 1000) / 1024;
	// Enable the compare match interrupt
	TIMSK |= (1 << OCIE1A);
}

unsigned long millis()
{
	unsigned long millisValue;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		millisValue = _timer1Counter;
	}
	return millisValue;
}

#endif