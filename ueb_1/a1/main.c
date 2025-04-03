#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

/*
 * 	PORT MAPPING:
 *
 * 	PD2: Button1
 *	PB0: Button2
 *	PD6: DIP1
 *	PD7: DIP2
 *	PB1: DIP3
 *
 *	PD3: LED1
 *	PD4: LED2
 *	PD5: LED3
 */


int main(void) {
	
	DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
	PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);

	int8_t countdown = 0x7;

	while(1) {
		PORTD &= ~((1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));
		PORTD |= (0xF & ~countdown) << PORTD3;
		if (countdown == 0) countdown = 7;
		else countdown--;
		_delay_ms(1000);
	}
}
