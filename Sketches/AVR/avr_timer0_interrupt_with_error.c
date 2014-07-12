#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// PB0 = led (blink at timer0 overflow)

ISR(TIMER0_OVF_vect) {

	// toggle PB0 state
	PORTB ^= _BV(PB0);
	
	_delay_ms(500);
}

int main (void) {
	
	// setup PB0 pin to output
	DDRB |= _BV(DDB0);
	
	// setup timer0 ( 8bit timer )
	TCNT0 = 0x00; // counter value
	TCCR0B = 0x05; // internal clock source with 1024 prescaler
	TIMSK |= _BV(TOIE0); // enable timer0 overflow interrupt
	
	// elapsed_ticks = (2^8 * PRESCALER)
	// elapsed_seconds = elapsed_ticks / F_CPU
	// now: 0,262144 seconds between interrupt
	
	// missing interrupt when ISR run more then elapsed_ticks
	// currently execute ISR is not executed again

	// enable global interrupts
	sei();
	
	while(1);
}
