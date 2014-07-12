#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// PB0 = led (blink at timer0 overflow)
// PB3 = button to ground (weakup event)

ISR(PCINT_vect) {

	PORTB ^= _BV(PB0);
}

int main (void) {
	
	// setup PB0 pin to output
	DDRB |= _BV(DDB0);
	
	// set pull up resistor
	PORTB |= _BV(PB3); 
	
	// setup pin change interrupt
	PCMSK |= _BV(PCINT3); // enable only pcint3
	GIMSK |= _BV(PCIE); // enable pin change interrupt

	// enable global interrupts
	sei();
	
	while(1);
}
