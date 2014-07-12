#define F_CPU 1000000UL
#define BAUDRATE 9600

#include <avr/io.h>
#include <avr/interrupt.h>

// PD0 = incoming, RX (tx port of the other device)
// PD1 = outgoing, TX (rx port of the other device)

#define UBRRX2 ((F_CPU / (8UL * BAUDRATE)) - 1)

ISR(USART_RX_vect) {
	// echo
	UDR = UDR;
}

int main (void) {
	
	// baud settings
	UBRRH = (uint8_t)(UBRRX2 >> 8);
	UBRRL = (uint8_t)UBRRX2;
	
	// enable transmit and receive
	UCSRB |= (_BV(TXEN) | _BV(RXEN));
	
	// enable receive complete interrupt
	UCSRB |= (_BV(RXCIE));
	
	// 8bit data
	UCSRC |= _BV(UCSZ1) | _BV(UCSZ0);
	
	// usart 2x mode
	UCSRA |= (_BV(U2X));
	
	sei();
	while(1);
}
