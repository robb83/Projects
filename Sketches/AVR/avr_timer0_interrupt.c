#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

// PB0 = led (blink at timer0 overflow)
// PB1 = led (blink at timer0 A comperator match)
// PB2 = led (blink at timer0 B comperator match)

ISR(TIMER0_COMPB_vect) {
	
	// toggle PB2 state
	PORTB ^= _BV(PB2);
}

ISR(TIMER0_COMPA_vect) {
	
	// toggle PB1 state
	PORTB ^= _BV(PB1);
}

ISR(TIMER0_OVF_vect) {

	// toggle PB0 state
	PORTB ^= _BV(PB0);
}

int main (void) {
	
	// setup PB0 pin to output
	DDRB |= _BV(DDB0);
	// setup PB1 pin to output
	DDRB |= _BV(DDB1);
	// setup PB2 pin to output
	DDRB |= _BV(DDB2);
	
	// setup timer0 ( 8bit timer )
	TCNT0 = 0x00; // counter value
	TCCR0B = 0x05; // internal clock source with 1024 prescaler
	TIMSK |= _BV(TOIE0); // enable timer0 overflow interrupt
	
	// elapsed_ticks = (2^8 * PRESCALER)
	// elapsed_seconds = elapsed_ticks / F_CPU
	// now: 0,262144 seconds between interrupt

	// setup timer0 compare interrupt A
	OCR0A = 0x55;
	TIMSK |= _BV(OCIE0A);
	
	// setup timer0 compare interrupt B
	OCR0B = 0xAA;
	TIMSK |= _BV(OCIE0B);

	// enable global interrupts
	sei();
	
	while(1);
}
