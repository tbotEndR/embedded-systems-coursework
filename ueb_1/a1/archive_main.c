#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

/*
PORT MAPPING: 
PORTD3: LED1 | PORTD4: LED2 | PORTD5: LED3
*/

int main(void)
{
	// init LEDs to hi
	DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
	PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
	uint8_t counter = 7;

	while(1)
	{
		if(counter < 0) counter = 0x7;
		PORTD &= ~((1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));	// reset LED pins
		PORTD |= (0xF & ~counter)<<PORTD3;
		counter--;
		_delay_ms(1000);	
	}
}
