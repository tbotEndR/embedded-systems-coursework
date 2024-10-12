#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

/*
PORT MAPPING: 
PORTD2: LED1 | PORTD3: LED2 | PORTD4: LED3
PIND5: START | PIND6: RESET | PIND7: DIP1 | PINB0: DIP2 | PINB1: DIP3
*/

int main(void)
{
	// init LEDs to hi
	DDRD |= (1<<DDD2) | (1<<DDD3) | (1<<DDD4);
	PORTD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4);
	// set pullups for inputs
	PORTD |= (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7);
	PORTB |= (1<<PORTB0) | (1<<PORTB1);
	int8_t counter = 7, dipInput = counter, tmp = 0x0, msTimer = 0;
	char startButtonDown = '0', countdown = '0', resetButtonDown = '0', reset = '0';

	while(1)
	{
		if (!(PIND & (1<<PIND5)))
		{
			if (startButtonDown == '0')
			{
				countdown = '1';
				reset = '0';
				startButtonDown = '1';
			}
		}
		else startButtonDown = '0';

		if (!(PIND & (1<<PIND6)))
		{
			if (resetButtonDown == '0')
			{
				reset = '1';
				countdown = '0';
				resetButtonDown = '1';
			}
		}
		else resetButtonDown = '0';

		if (reset == '1')
		{
			msTimer = 0;
			dipInput = (((PIND&(1<<PIND7))>>5) | (PINB&(1<<PINB1))>>1 | (PINB&(1<<PINB0))<<1);
			counter = dipInput;
			tmp = ((PORTD & (~(0b111<<PORTD2))) | (~counter&0x07)<<PORTD2);
			PORTD = tmp;
		}

		if ((msTimer >= 10) && (countdown == '1'))
		{
			if (counter < 0)
			{
				counter = dipInput;
			}
			PORTD &= ~((1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4));	// reset LED pins
			PORTD |= (0xF & ~counter)<<PORTD2;
			counter--;
			msTimer = 0;
		}
		msTimer++;
		_delay_ms(100);		
	}
}
