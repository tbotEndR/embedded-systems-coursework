#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);              // set DDD2-4 to output
    PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);       // set output hi
    PORTD |= (1<<PORTD2) | (1<<PORTD6) | (1<<PORTD7);       // set pullup resistor for start, reset, dip1
    PORTB |= (1<<PORTB0) | (1<<PORTB1);                     // set pullup resistor for dip2, dip3

    while(1)
    {
        if (PIND & (1<<PIND2))
        {
            PORTD |= (1<<PORTD3);
        }
        else PORTD &= ~(1<<PORTD3);

        if (PIND & (1<<PIND6))
        {
            PORTD |= (1<<PORTD4);
        }
        else PORTD &= ~(1<<PORTD4);

        if (PINB & (1<<PINB0))
        {
	    PORTD |= (1<<PORTD5);
        }
	else PORTD &= ~(1<<PORTD5);
        _delay_ms(100);
    }

    return 0;
}
