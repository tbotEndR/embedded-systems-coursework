#ifndef USART_DRIVER_H
#define USART_DRIVER_H
#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)

#include <avr/interrupt.h>


/*	---------------------------------------------------------------*/
//	A simple USART driver. Interrupt triggers when receiving bytes and toggles
//	outgoing communication according to PC xoff/xon. Other characters are passed
//	to the user in the udr_input variable.
/*	---------------------------------------------------------------*/


//	xon 0x11
//	xoff 0x13

volatile unsigned char udr_input = 0;
volatile unsigned char udr_output = 0;
int8_t xoff_received = 0;
int8_t xoff_sent = 0;

/* ---------------------------------------------------------------*/


ISR(USART_RX_vect) {
	udr_input = UDR0;
	if (udr_input == 0x13) {
		xoff_received = 1;
		udr_input = 0;
	} else if (udr_input == 0x11) {
		xoff_received = 0;
		udr_input = 0;
	}
}


void USART_Init(){
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);		//	enable receive and transmit
	UCSR0B |= (1<<RXCIE0);					//	enable receive interrupt
	sei();
}


void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	if (!xoff_received) UDR0 = data;
}


#endif