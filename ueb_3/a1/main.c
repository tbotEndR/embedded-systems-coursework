#define F_CPU 16000000L
#define STARTPRESSED_F 0
#define RESETPRESSED_F 1
#define TIMERSTART_F 2
#define TIMERRESET_F 3
#define DIPLIMIT_REG 4
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
 
int8_t countdown = 0x07;
uint8_t flags = 0x00;
uint8_t milliseconds = 0;
uint8_t fifties = 0;				// how many 50ms have passed

/* 	
	Bit0: StartPressed | Bit1: ResetPressed
    Bit2: TimerStarted | Bit3: TimerReset  
    Bits 4-7: User Input Limit
*/

ISR(PCINT2_vect)
{
    //	reset counter back to user defined limit
    countdown = (flags >> DIPLIMIT_REG);
}

ISR(TIMER0_OVF_vect)
{
	//	timer interrupt is called every 4us after its 8bit counter (TCNT0) overflows
	//	set TCNT0 to 6 so it overflows every 250 ticks aka 1ms
	TCNT0 = 6;									
	milliseconds++;
}

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
    //	set timer0 prescaler to 64
    TCCR0B |= ((1<<CS01) | (1<<CS00));
    //	enable timer overflow interrupt
    TIMSK0 |= (1<<TOIE0);
    sei();
    flags |= (countdown << DIPLIMIT_REG);

    while(1)
    {
		/*	Check Buttons 	*/

		if (milliseconds >= 50) {
			// check buttons
			if (!(PIND & (1<<PIND2))) {
				if (!(flags & (1<<STARTPRESSED_F))) {
					flags |= (1<<STARTPRESSED_F);	
					flags |= (1<<TIMERSTART_F);	
				}
			} else flags &= ~(1<<STARTPRESSED_F);

			if (!(PINB & (1<<PINB0))){
				if (!(flags & (1<<RESETPRESSED_F))) {
					flags |= (1<<RESETPRESSED_F);
					flags |= (1<<TIMERRESET_F);
				}
			} else flags &= ~(1<<RESETPRESSED_F); 

			fifties++;
			milliseconds = 0;
		}

		/*	Timer Started State:
		 *	- output countdown bits on leds 1-3
		 * 	- decrement countdown every 1s		*/

		if ((flags & (1<<TIMERSTART_F)) && (fifties >= 20))
		{
			flags &= ~(1<<TIMERRESET_F);
			if (countdown < 0) PORTD ^= (1<<PORTD0);
			PORTD &= ~(0x7 << PORTD3);
			PORTD |= (0x07 & ~countdown) << PORTD3;
			countdown--;
			fifties = 0;
		}

		/*	Timer Reset State:
		 *	- read user input from dip switches 1-3
		 *	- save user input as new countdown limit		*/

		if (flags & (1<<TIMERRESET_F))
		{
			flags &= ~(1<<TIMERSTART_F);
			countdown = ~((PIND & (1<<PIND6))>>4 | (PIND & (1<<PIND7))>>6 | (PINB & (1<<PINB1))>>1) & 0x07;
			flags |= (0x0F << DIPLIMIT_REG);
			flags &= (countdown << DIPLIMIT_REG) | 0x0F;
			countdown = (flags >> DIPLIMIT_REG) & 0x07;
			PORTD &= ~(0x7 << PORTD3);
			PORTD |= (~countdown << PORTD3);
		}

    }
}
