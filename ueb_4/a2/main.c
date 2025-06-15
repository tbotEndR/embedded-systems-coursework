#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ringbuf.h"
#include "usart_driver.h"


int main(void)
{
	ringbuffer32_t rb;
	ringbufferInit(&rb);
	uint8_t free_cells = RINGBUFFERSIZE;
	
	const char meldung[]="Hier ATmega. Wer da?";
	USART_Init();
	USART_Transmit('\n');
	for(int f=0;meldung[f]!='\0';f++)
		USART_Transmit(meldung[f]);
		
    while (1) 
    {
		if (udr_input != 0) {
			if (udr_input != 0x0d){
				push(&rb, udr_input);
				USART_Transmit(tail(&rb));
				udr_input = 0;
				if (--free_cells < 1) {
					USART_Transmit(0x13);
					udr_input = 0x0d;
				}
			} else {
				USART_Transmit(0x0d);
				USART_Transmit('\n');
				USART_Transmit('H');
				USART_Transmit('i');
				USART_Transmit(' ');
				while(head(&rb) != 0) {
					USART_Transmit(pop(&rb));
					if (++free_cells > 30) USART_Transmit(0x11);
				}
				USART_Transmit(0x0d);
				USART_Transmit('\n');
				udr_input = 0;
				_delay_ms(1000);
			}
		}
    }
}

