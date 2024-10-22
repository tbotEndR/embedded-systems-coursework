#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
PORT MAPPING: 
PORTD3: LED1 | PORTD4: LED2 | PORTD5: LED3
PIND6: DIP1 | PIND7: DIP2 | PINB0: DIP3 | PINB1: RESET | PIND2: START_Interrupt 
*/

volatile int8_t counter = 7, dipInput = 7, tmp = 0x0, msTimer = 0;
volatile char startButtonDown = '0', countdown = '1', resetButtonDown = '0', reset = '0';

// start
ISR(INT0_vect)
{
	countdown = '1';
	reset = '0';
}

// reset
ISR(PCINT1_vect)
{
	reset = '1';
	countdown = '0';
}

int main(void)
{
	// init LEDs to hi
	DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
	PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
	// set pullups for inputs
	PORTD |= (1<<PORTD2) | (1<<PORTD6) | (1<<PORTD7);
	PORTB |= (1<<PORTB0) | (1<<PORTB1);
	PORTC |= (1<<PORTC2);
	// configure interrupt rising edge detection
	PCICR &= ~(1<<PCIE1);
	PCICR |= (1<<PCIE1);
	EICRA &= ~((1<<ISC01) | (1<<ISC00));
	EICRA |= (1<<ISC01);
	// enable INT0 interrupt
	EIMSK |= (1<<INT0);
	// enable PCINT1 interrupt
	PCMSK1 |= (1<<PCINT10);
	// enable interrupts beside reset
	sei();

	while(1)
	{
		if (reset == '1')
		{
			msTimer = 0;
			dipInput = ((PIND&(1<<PIND6))>>4 | (PIND&(1<<PIND7))>>6 | (PINB&(1<<PINB0)));
			counter = dipInput;
			tmp = ((PORTD & (~(0b111<<PORTD3))) | (~counter&0x07)<<PORTD3);
			PORTD = tmp;
		}

		if ((msTimer >= 10) && (countdown == '1'))
		{
			if (counter < 0)
			{
				counter = dipInput;
			}
			PORTD &= ~((1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));	// reset LED pins
			PORTD |= (0xF & ~counter)<<PORTD3;
			counter--;
			msTimer = 0;
		}
		
		msTimer++;
		_delay_ms(100);		
	}
}
