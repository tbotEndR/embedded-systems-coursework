#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <util/delay.h>
#include "ringbuf.h"


void USART_Init(){
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}


void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = data;
}


unsigned char USART_Receive(){
	while(!(UCSR0A & (1<<RXC0))) ;
	return UDR0;
}


int main(void)
{
	ringbuffer32_t rb;
	ringbufferInit(&rb);
	
	const char meldung[]="Hier ATmega. Wer da?";
	USART_Init();
	USART_Transmit('\n');
	for(int f=0;meldung[f]!='\0';f++)
		USART_Transmit(meldung[f]);
		
    while (1) 
    {
		push(&rb, USART_Receive());
		if (last(&rb) != 0x0d) {
			USART_Transmit(last(&rb));
			continue;
		}
		
		USART_Transmit('\n');
		USART_Transmit('H');
		USART_Transmit('i');
		USART_Transmit(' ');
		while(first(&rb) != 0)
			USART_Transmit(pop(&rb));
		USART_Transmit('\n');

    }
}

