#ifndef TIMERS_H
#define TIMERS_H
#define MAXTIMERLENUSEC 10000000
#define MAXTIMERS 5
#include <avr/interrupt.h>


/* -------------------------------------------------------------------------------------------------------------------------- */


volatile uint8_t half_millisec_counter = 0;

typedef void (*callback)();
typedef uint8_t timer_t;
typedef struct timer_struct {
	callback cb;
	volatile uint16_t ms_clock;
	uint16_t limit;
	uint8_t active;
} timer_struct;
timer_struct timers[MAXTIMERS];


/* -------------------------------------------------------------------------------------------------------------------------- */


void timerh_interrupt_init() {
	//	enable timer1 with a 64 prescaler
	TCCR1B |= (1<<CS11) |(1<<CS10);	
	TIMSK1 |= (1<<TOIE1);
}


void declareTimer(timer_t timer, uint16_t limit, callback cb) {
	if (timer < MAXTIMERS){
		timers[timer].limit = limit;
		timers[timer].cb = cb;
		timers[timer].active = 0;
	}
}


void startTimer(timer_t timer) {
	if (timer < MAXTIMERS) {
		//active_timers |= (1<<timer);
		timers[timer].ms_clock = 0;
		timers[timer].active = 1;
	}
}


void cancelTimer(timer_t timer) {
	if (timer < MAXTIMERS)	//active_timers &= !(1<<timer);
		timers[timer].active = 0;
}


void checkTimers() {	
	for (int i = 0; i < MAXTIMERS; i++) {
		//if (active_timers & (1<<i)) {
		if (timers[i].active) {
			if (timers[i].limit <= timers[i].ms_clock) {
				cli();
				timers[i].cb();
				cancelTimer(i);
				sei();
			}
		}
	}
}


uint8_t timerStarted(timer_t timer) {
	//return active_timers & (1<<timer) ? 1 : 0;
	return timers[timer].active;
}


ISR(TIMER1_OVF_vect) {
	cli();
	TCNT1 = 65411;			//	overflows every 125 ticks = 500us (4us per tick with a 64 prescaler at 16Mhz)
	if (++half_millisec_counter > 1) {
		half_millisec_counter = 0;	
		for (int i = 0; i < MAXTIMERS; i++) {
			if (timers[i].active) {
				timers[i].ms_clock++;
			}
		}
	}
	sei();
}

#endif
