#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"


/*
- - - INPUTS - - - 
PIND2: START 
PIND6: DIP1 
PIND7: DIP2
PINB0: RESET  
PINB1: DIP3

- - - OUTPUTS - - -
** Ascending right to left **
PORTD0: LED OVERFLOW (PCINT16)
PORTD3: LED1
PORTD4: LED2
PORTD5: LED3
*/

volatile int8_t countdown = 0x07;									 
volatile uint8_t dip_limit_input = 0x00;
char start_state = '0';
char start_pressed = '0';
char reset_state = '0';
char reset_pressed = '0';

ISR(PCINT2_vect) {
    //	reset counter back to user defined limit
    countdown = dip_limit_input;
}

void checkButtons();
void updateLEDs();

int main(void)
{
    // OUTPUTS
    DDRD |= (1<<DDD0) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
    PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
    // INPUTS
    PORTD |= (1<<PORTD2) | (1<<PORTD6) | (1<<PORTD7);
    PORTB |= (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2);
    /*	Interrupt setup */
    //	enable pin change interrupt vector 2
    PCICR &= ~(1<<PCIE2);
    PCICR |= (1<<PCIE2);
    //	enable pcpint16 in pcint2_vect
    PCMSK2 |= (1<<PCINT16);
	timerh_interrupt_init();
    sei();
	
	
	declareTimer(0, 50, &checkButtons);
	declareTimer(1, 1000, &updateLEDs);
		

    while(1)
    {
		if (!timerStarted(1)) startTimer(1);
		if (!timerStarted(0)) startTimer(0);
		checkTimers();
	}
}

void checkButtons() {
	/*	Update state */
	if (!(PIND & (1<<PIND2))) {
		if (start_pressed == '0') {
			start_pressed = '1';
			reset_state = '0';
			start_state = '1';
		}
	} else start_pressed = '0';
	if (!(PINB & (1<<PINB0))){
		if (reset_state == '0') {
			reset_pressed = '1';
			start_state = '0';
			reset_state = '1';
		}
	} else reset_pressed = '0';
	
	/*	Update dip input if reset state */
	if (reset_state == '1') {
		dip_limit_input = ~((PIND & (1<<PIND6))>>4 | (PIND & (1<<PIND7))>>6 | (PINB & (1<<PINB1))>>1) & 0x07;
		countdown = dip_limit_input;
		PORTD &= ~(0x7 << PORTD3);
		PORTD |= (0x07 & ~countdown) << PORTD3;
	}
	
	if (start_state == '1') {
		if (--countdown < 0) PORTD |= (1<<PORTD0);
	}
}

void updateLEDs() {
	PORTD &= ~(0x7 << PORTD3);
	PORTD |= (0x07 & ~countdown) << PORTD3;
}
