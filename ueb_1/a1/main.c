#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

/*
PORT MAPPING
PORTD3: LED1 | PORTD4: LED2 | PORTD5: LED1
*/

int main(void)
{
    DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
    PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);

    uint8_t counter_var = 0x07;

    while(1)
    {
	if (counter_var < 0) counter_var = 0x07;
	PORTD &= ~((1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));
	PORTD |= (0xF & ~counter_var) << PORTD3;
	counter_var--;
	_delay_ms(1000);
    }
}
