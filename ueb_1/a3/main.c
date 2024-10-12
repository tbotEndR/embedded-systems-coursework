#define F_CPU 16000000L
#define STARTPRESS_F 0
#define RESETPRESS_F 1
#define TIMERSTART_F 2
#define TIMERRESET_F 3
#define DIPLIMIT_REG 4
#include <avr/io.h>
#include <util/delay.h>

/*
- - - INPUTS - - - 
PIND2: START
PIND6: RESET
PIND7: DIP1
PINB0: DIP2
PINB1: DIP3

- - - OUTPUTS - - -
** Ascending right to left **
PORTD3: LED1
PORTD4: LED2
PORTD5: LED3
*/


int main(void)
{
    // OUTPUTS
    DDRD |= (1<<DDD3) | (1<<DDD4) | (1<<DDD5);
    PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
    // INPUTS
    PORTD |= (1<<PORTD2) | (1<<PORTD6) | (1<<PORTD7);
    PORTB |= (1<<PORTB0) | (1<<PORTB1);

    uint8_t flags = 0x00;
    /* 	Bit0: StartPressed | Bit1: ResetPressed 
	Bit2: TimerStarted | Bit3: TimerReset  
	Bits 4-7: User Input Limit
    */
    int8_t countdown = 0x07, loop_counter = 0x00;
    flags |= (countdown << DIPLIMIT_REG);

    while(1)
    {
	/*	Start Button Logic  */
	if (!(PIND & (1<<PIND2)) && !(flags & (1<<STARTPRESS_F)))
	{
	    flags |= (1<<STARTPRESS_F) | (1<<TIMERSTART_F);
	    flags &= ~(1<<TIMERRESET_F);
	}
	else if ((PIND & (1<<PIND2)) && (flags & (1<<STARTPRESS_F)))
	{
	    flags &= ~(1<<STARTPRESS_F);
	}

	/*	Reset Button Logic  */
	if (!(PIND & (1<<PIND6)) && !(flags & (1<<RESETPRESS_F)))
	{
	    flags |= (1<<RESETPRESS_F) | (1<<TIMERRESET_F);
	    flags &= ~(1<<TIMERSTART_F);
	}
	else if ((PIND & (1<<PIND6)) && (flags & (1<<RESETPRESS_F)))
	{
	    flags &= ~(1<<RESETPRESS_F);
	}

	/*	Timer Started State  */
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

	/*	Timer Reset State  */
	if (flags & (1<<TIMERRESET_F))
	{
	    countdown = ~((PIND & (1<<PIND7))>>5 | (PINB & (1<<PINB0))<<1 | (PINB & (1<<PINB1))>>1) & 0x07;
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
