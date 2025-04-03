#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(PCINT0_vect) 
{
    PORTD ^= (1<<PORTD3);
}

int main(void)
{
    /* LED */
    DDRD |= (1<<DDD0) | (1<<DDD3) ;        
    PORTD |= (1<<PORTD0) | (1<<PORTD3) ;      
    DDRB |= (1<<DDB2);

    /* BUTTON */
    PORTD |= (1<<PORTD2);

    PCICR |= (1<<PCIE0);
    PCMSK0 |= (1<<PCINT2);
    sei();

    while(1)
    {
	if (!(PIND & (1<<PIND2))) PORTB ^= (1<<PORTB2);
        _delay_ms(250);
    }

    return 0;
}
