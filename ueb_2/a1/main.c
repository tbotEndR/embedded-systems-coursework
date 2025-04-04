#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "flagops.h"

#define START_F 3
#define RESET_F 4
#define STARTPRESSED_F 5
#define RESETPRESSED_F 6

/*
 * 	PORT MAPPING:
 *
 * 	PD2 (INT0): Button1 (Start)
 *	PC2 (PCINT10): Button2 (Reset)
 *	PD6: DIP1 = flag bit 2
 *	PD7: DIP2 = flag bit 1
 *	PB1: DIP3 = flag bit 0
 *
 *	PD3: LED1
 *	PD4: LED2
 *	PD5: LED3
*/

ISR(INT0_vect) {
	setFlag(START_F);
	clearFlag(RESET_F);
}

ISR(PCINT1_vect) {
	setFlag(RESET_F);
	clearFlag(START_F);
}

int main(void) {
	// 	LED Setup	
	DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
	PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
	//	Button Pullup Setup
	PORTB |= (1<<PORTB1);
	PORTD |= (1<<PORTD2) | (1<<PORTD6) | (1<<PORTD7);
	PORTC |= (1<<PORTC2);
	// Interrupt Setup
	EICRA |= (1<<ISC01) | (1<<ISC00);		//	rising edge	
	EIMSK |= (1<<INT0);						//	enable int0
	
	PCICR |= (1<<PCIE1);					//	enable pci1 interrupt vector
	PCMSK1 |= (1<<PCINT10); 				//	enable pcint10
	
	sei();

	int8_t delayCounter = 0x0;
	flag_reg |= 0x07;
	int8_t countdown = (flag_reg & 0x07);

	while(1) {

		if (delayCounter >= 10) {
			//	write to LEDs
			PORTD &= ~((1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));
			PORTD |= (0xF & ~countdown) << PORTD3;
			if (flagSet(START_F)) {
				if (countdown == 0) countdown = (flag_reg & 0x07);
				else countdown--;
			}
			delayCounter = 0;
		}

		else if (flagSet(RESET_F)) {
			//	use countdown variable as temporary dip-input
			countdown =  ~(((PIND & (1<<PIND6)) >> 4) | ((PIND & (1<<PIND7)) >> 6) | ((PINB & (1<<PINB1)) >> 1)) & 0x07;
			flag_reg |= 0x07;
			flag_reg &= countdown | 0xF8;
		}

		delayCounter++;	
		_delay_ms(100);
	}
}
