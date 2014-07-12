#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// PB0 = led (blink at timer0 overflow)

ISR(TIMER0_OVF_vect) {
	PORTB ^= _BV(PB0);
}

int main (void) {
	
	// setup PB0 pin to output
	DDRB |= _BV(DDB0);
	
	// setup timer0
	TCNT0 = 0x00; // counter value
	TCCR0B = 0x05; // 1024 divider
	TIMSK |= _BV(TOIE0); // enable interrupt

	// enable global interrupts
	sei();
	
	while(1);
}
