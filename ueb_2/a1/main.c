#define F_CPU 16000000L
#define TIMERSTART_F 2
#define TIMERRESET_F 3
#define DIPLIMIT_REG 4
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
- - - INPUTS - - - 
PIND2: START (INT0)
PIND6: DIP1 
PIND7: DIP2
PINB0: RESET (PCINT0) 
PINB1: DIP3

- - - OUTPUTS - - -
** Ascending right to left **
PORTD3: LED1
PORTD4: LED2
PORTD5: LED3
*/

uint8_t flags = 0x00;
/* 	
    Bit2: TimerStarted | Bit3: TimerReset  
    Bits 4-7: User Input Limit
*/

ISR(INT0_vect)
{
    /*	Start Button Logic:
     *	- set TimerStarted flag		*/

    flags |= (1<<TIMERSTART_F);
    flags &= ~(1<<TIMERRESET_F);
}

ISR(PCINT0_vect)
{
    /*	Reset Button Logic:
     *	- set TimerReset flag		*/

	flags |=  (1<<TIMERRESET_F);
	flags &= ~(1<<TIMERSTART_F);
}


int main(void)
{
    // OUTPUTS
    DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
    PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
    // INPUTS
    PORTD |= (1<<PORTD2) | (1<<PORTD6) | (1<<PORTD7);
    PORTB |= (1<<PORTB0) | (1<<PORTB1);
    /*	Interrupt setup */
    EICRA &= ~((1<<ISC01) | (1<<ISC00));
    EICRA |= (1<<ISC01);
    EIMSK |= (1<<INT0);
    PCICR &= ~(1<<PCIE0);
    PCICR |= (1<<PCIE0);
    PCMSK0 |= (1<<PCINT0);
    sei();

    int8_t countdown = 0x07, loop_counter = 0x00;
    flags |= (countdown << DIPLIMIT_REG);

    while(1)
    {
	/*	Timer Started State:
	 *	- output countdown bits on leds 1-3
	 * 	- decrement countdown every 1s		*/

	if (flags & (1<<TIMERSTART_F))
	{
	    if (loop_counter >= 10)
	    {
		if (countdown < 0)  countdown = (flags >> DIPLIMIT_REG);
		PORTD &= ~(0x7 << PORTD3);
		PORTD |= (0x07 & ~countdown) << PORTD3;
		countdown--;
		loop_counter = 0;
	    }
	}

	/*	Timer Reset State:
	 *	- read user input from dip switches 1-3
	 *	- save user input as new countdown limit		*/

	if (flags & (1<<TIMERRESET_F))
	{
	    countdown = ~((PIND & (1<<PIND6))>>4 | (PIND & (1<<PIND7))>>6 | (PINB & (1<<PINB1))>>1) & 0x07;
	    flags |= (0x0F << DIPLIMIT_REG);
	    flags &= (countdown << DIPLIMIT_REG) | 0x0F;
	    countdown = (flags >> DIPLIMIT_REG) & 0x07;
	    PORTD &= ~(0x7 << PORTD3);
	    PORTD |= (~countdown << PORTD3);
	}
	loop_counter++;
	_delay_ms(100);

    }
}
