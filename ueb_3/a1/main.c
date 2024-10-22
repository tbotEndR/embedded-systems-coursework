#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
PORT MAPPING: 
PORTD3: LED1 | PORTD4: LED2 | PORTD5: LED3 | PORTD6: OVERFLOW_Interrupt
PIND2: START_Interrupt | PIND7: DIP1 | PINB0: DIP2 | PINB1: DIP3 
PINC2: RESET_Interrupt
*/