#define F_CPU 1000000UL
#define BAUDRATE 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// PD0 = incoming, RX (tx port of the other device)
// PD1 = outgoing, TX (rx port of the other device)

#define UBRRX2 ((F_CPU / (8UL * BAUDRATE)) - 1)

#define USART_BUFFER_SIZE 16
unsigned char usart_buffer[USART_BUFFER_SIZE];
volatile uint8_t buffer_tail = 0;
volatile uint8_t buffer_head = 0;

void tx_char(char ch) {
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = ch;
}

void tx_string(char* string) {
	if (!string) return;
	
	unsigned char current;
	
	while((current = *string++) != '\0') {
		while ( !( UCSRA & (1<<UDRE)) );
		UDR = current;
	}
}

int rx() {
	if (buffer_tail != buffer_head) {
		buffer_tail = (buffer_tail + 1) % USART_BUFFER_SIZE;
		return usart_buffer[buffer_tail];		
	}
	
	return -1;
}

ISR(USART_RX_vect) {
	
	//TODO: frame error, data overrun, parity check
	
	unsigned char received = UDR;
	
	int pos = (buffer_head + 1) % USART_BUFFER_SIZE;
	
	if (pos != buffer_tail) {
		usart_buffer[pos] = received;
		buffer_head = pos;
	}
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
	
	tx_string("Hello World");
	
	while(1) {
		int r;
		while((r = rx()) != -1) {
			tx_char((char)r);
		}
		
		_delay_ms(10);
	}
}
