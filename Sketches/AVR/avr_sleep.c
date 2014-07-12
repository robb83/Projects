#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

// PB0 = led (blink at timer0 overflow)
// PB1 = led (lights while awake)
// PD2 = button to ground (weakup event)

void powerup() {
	TIMSK |= _BV(TOIE0);
	PORTB |= _BV(PB1);
}

void powerdown() {
	TIMSK &= ~_BV(TOIE0);
	PORTB &= ~_BV(PB1);
	PORTB &= ~_BV(PB0);
}

void gosleep() {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	
	powerdown();
	
	sleep_mode();
	sleep_disable();
	
	powerup();
}

ISR(INT0_vect) {
	// do nothing
}

ISR(TIMER0_OVF_vect) {

	// toggle PB0 state
	PORTB ^= _BV(PB0);
}

int main (void) {

	// setup pin to output
	DDRB |= _BV(DDB0);
	DDRB |= _BV(DDB1);
	
	// set pull up resistor
	PORTD |= _BV(PD2);
	
	// enable timer0 overflow interrupt
	TCNT0 = 0x00;
	TCCR0B = 0x05;
	TIMSK |= _BV(TOIE0);
	
	// enable int0 external interrupt
	GIMSK |= _BV(INT0);
	sei();
	
	powerup();

	while(1) {
		_delay_ms(4000);
		gosleep();
	}
}
