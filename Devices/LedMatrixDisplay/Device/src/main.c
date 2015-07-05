#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// TODO: state timeout, reset

#define USART_BAUDRATE 250000   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define BUTTON_RELEASED(PIN) ((pin_changed & (1 << PIN)) && (pin_interrupts & (1 << PIN) ))

#define DISPLAY_WIDTH (32)
#define DISPLAY_HEIGHT (16)
#define DISPLAY_BUFFER_BIT (8)
#define DISPLAY_BUFFER_COLUMN (DISPLAY_WIDTH / DISPLAY_BUFFER_BIT)
#define DISPLAY_BUFFER_LAST_LINE ((DISPLAY_HEIGHT - 1) * DISPLAY_BUFFER_COLUMN)
#define DISPLAY_BUFFER_CENTER_LINE (DISPLAY_HEIGHT / 2)
#define DISPLAY_BUFFER_CENTER (34)

volatile uint8_t buffer[64];
volatile uint8_t state = 0x00;
volatile uint8_t state_position = 0x00;
volatile uint8_t state_buffer[4];
volatile uint8_t pin_interrupts = 0xFF;
volatile uint8_t pin_changed = 0x00;

ISR (PCINT_vect) {
	uint8_t buttons = PINB & 0b00011000;
	
    pin_changed = buttons ^ pin_interrupts;
    pin_interrupts = buttons;
}

ISR(USART_RX_vect) {
	if (state == 0x00) {
		state = UDR;
		state_position = 0x00;
	} else if (state == 'A') {
		if (state_position < 64) {
			buffer[state_position++] = UDR;
		}
	} else if (state == 'B') {
		if (state_position < 2) {
			state_buffer[state_position++] = UDR;
		}
	} else if (state == 'C') {
		if (state_position < 2) {
			state_buffer[state_position++] = UDR;
		}
	} else if (state == 'F') {
		if (state_position < 4) {
			state_buffer[state_position++] = UDR;
		}
	} else if (state == 'G') {
		if (state_position < 4) {
			state_buffer[state_position++] = UDR;
		}
	}
}

void uart_send(uint8_t data) {
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = data;
}

void display_clear(uint8_t v) {
	for(int i = 0; i < sizeof(buffer); ++i) {
		buffer[i] = v;
	}
}

void display_shift_right(void) {
	uint8_t r, c;
	for ( r = 0; r < DISPLAY_HEIGHT; ++r) {
		
		uint8_t t1 = 0;
		for( c = r * DISPLAY_BUFFER_COLUMN; c < r * DISPLAY_BUFFER_COLUMN + 4; ++c) {
			uint8_t t2 = buffer[c];
			
			uint8_t t3 = (t2 & 0b00000001) << 7;
			
			t2 = (t2 >> 1) & 0b01111111;
			t2 = t2 | t1;
			
			buffer[c] = t2;
			
			t1 = t3;
		}
	}
}

void display_shift_left(void) {
	int8_t r, c;
	for ( r = 0; r < DISPLAY_HEIGHT; ++r) {
		
		uint8_t t1 = 0;
		for( c = (DISPLAY_BUFFER_COLUMN - 1); c >= 0; --c) {
			uint8_t t2 = buffer[r * DISPLAY_BUFFER_COLUMN + c];
			
			uint8_t t3 = (t2 & 0b10000000) >> 7;
			
			t2 = (t2 << 1) & 0b11111110;
			t2 = t2 | t1;
			
			buffer[r * DISPLAY_BUFFER_COLUMN + c] = t2;
			
			t1 = t3;
		}
	}
}

void display_shift_up(void) {
	uint8_t r;
	for( r = 0; r < DISPLAY_BUFFER_LAST_LINE; ++r) {
		buffer[r] = buffer[r + 4];
	}
	for( r = DISPLAY_BUFFER_LAST_LINE; r < sizeof(buffer); ++r) {
		buffer[r] = 0x00;
	}
}

void display_shift_down(void) {
	int8_t r;
	for( r = DISPLAY_BUFFER_LAST_LINE - 1; r >= 0; --r) {
		buffer[r + DISPLAY_BUFFER_COLUMN] = buffer[r];
	}
	for( r = 0; r < DISPLAY_BUFFER_COLUMN; ++r) {
		buffer[r] = 0x00;
	}
}

void display_plotting(void) {
	static uint8_t r = 0;
	static uint8_t c = 0;
	static uint8_t b = 0;
	
	for ( r = 0; r < DISPLAY_HEIGHT; ++r) {
		for( c = 0; c < DISPLAY_BUFFER_COLUMN; ++c) {			
			for(b = 0; b < DISPLAY_BUFFER_BIT; ++b) {
				PORTB &= ~(1 << (PB2));
				
				if (buffer[r * 4 + c] & (1 << (7 - b)))
					PORTB &= ~(1 << (PB0));
				else
					PORTB |= (1 << (PB0));
				
				PORTB |= (1 << (PB2));
			}
		}
		
		PORTD |= (1 << (PD6));
		
		PORTD = (PORTD & 0b11000011) | ((r & 0b00001111) << 2);
		
		PORTB &= ~(1 << (PB1));
		PORTB |= (1 << (PB1));
		
		PORTD &= ~(1 << (PD6));
	}
}

int main(void) {

	// initialize microcontroller
	TCCR1B |= (1 << CS10) | (1 << CS11);
	
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8); 
	UCSRB = ((1<<TXEN) | (1<<RXEN) | (1 << RXCIE));
	
	DDRD = 0b01111110;
	DDRB = 0b00000111;
	
	PORTB |= (( 1 << PCINT3 ) | ( 1 << PCINT4 ));
	PCMSK |= (( 1 << PCINT3 ) | ( 1 << PCINT4 ));
	GIMSK |= ( 1 << PCIE );
	
	// initialize display
	display_clear(0x00);
	buffer[DISPLAY_BUFFER_CENTER - DISPLAY_BUFFER_COLUMN - 1] = 0b00000001;
	buffer[DISPLAY_BUFFER_CENTER - DISPLAY_BUFFER_COLUMN] = 0b10000000;
	buffer[DISPLAY_BUFFER_CENTER - 1] = 0b00000001;
	buffer[DISPLAY_BUFFER_CENTER] = 0b10000000;
	
	// enable interrupts
	sei();
	
    while (1) {
		
		if (pin_changed) {
			uart_send('Z');
			uart_send(pin_interrupts);
			
			// lamp mode
			if (BUTTON_RELEASED(PB3)) {
				if (state == 'Y') {
					display_clear(0x00);
					state = 0x00;
				} else {
					if (state != 0x00) {
						uart_send(state);
					}
					state = 'Y';
					display_clear(0xFF);
				}
			}
			
			pin_changed = 0x00;
		}
		
		// display plotting
		if (state == 0x00 || state == 'Y') {
			display_plotting();
			
		// fill display buffer
		} else if (state == 'A') {
			if (state_position >= 64) {
				state = 0x00;
				uart_send('A');
			}
			
		// display shift right
		} else if (state == 'B') {
			if (state_position >= 2) {
				uint8_t a = state_buffer[0];
				uint8_t b = state_buffer[1];
				
				display_shift_right();
				for (uint8_t c = 0; c < DISPLAY_BUFFER_BIT; ++c) {
					uint8_t d = buffer[c * DISPLAY_BUFFER_COLUMN];
					if (a & (1 << ((DISPLAY_BUFFER_BIT - 1) - c))) {
						d |= 0b10000000;
					}
					buffer[c * DISPLAY_BUFFER_COLUMN] = d;
				}
				for (uint8_t c = 0; c < DISPLAY_BUFFER_BIT; ++c) {
					uint8_t d = buffer[(c + DISPLAY_BUFFER_BIT) * DISPLAY_BUFFER_COLUMN];
					if (b & (1 << ((DISPLAY_BUFFER_BIT - 1) - c))) {
						d |= 0b10000000;
					}
					buffer[(c + DISPLAY_BUFFER_BIT) * DISPLAY_BUFFER_COLUMN] = d;
				}
				state = 0x00;
				uart_send('B');
			}
			
		// display shift left
		} else if (state == 'C') { 
			if (state_position >= 2) {
				uint8_t a = state_buffer[0];
				uint8_t b = state_buffer[1];
					
				display_shift_left();
				for (uint8_t c = 0; c < DISPLAY_BUFFER_BIT; ++c) {
					uint8_t d = buffer[c * DISPLAY_BUFFER_COLUMN + (DISPLAY_BUFFER_COLUMN - 1)];
					if (a & (1 << ((DISPLAY_BUFFER_BIT - 1) - c))) {
						d |= 0b00000001;
					}
					buffer[c * DISPLAY_BUFFER_COLUMN + (DISPLAY_BUFFER_COLUMN - 1)] = d;
				}
				for (uint8_t c = 0; c < DISPLAY_BUFFER_BIT; ++c) {
					uint8_t d = buffer[(c + DISPLAY_BUFFER_BIT) * DISPLAY_BUFFER_COLUMN + (DISPLAY_BUFFER_COLUMN - 1)];
					if (b & (1 << ((DISPLAY_BUFFER_BIT - 1) - c))) {
						d |= 0b00000001;
					}
					buffer[(c + DISPLAY_BUFFER_BIT) * DISPLAY_BUFFER_COLUMN + (DISPLAY_BUFFER_COLUMN - 1)] = d;
				}
				state = 0x00;
				uart_send('C');
			}
			
		// display turn off
		} else if (state == 'D') { 
			display_clear(0x00);
			state = 0x00;
			uart_send('D');
			
		// display turn on
		} else if (state == 'E') { 
			display_clear(0xFF);
			state = 0x00;
			uart_send('E');
			
		// display shift down
		} else if (state == 'F') { 
			if (state_position >= 4) {
				display_shift_down();
				buffer[0] = state_buffer[0];
				buffer[1] = state_buffer[1];
				buffer[2] = state_buffer[2];
				buffer[3] = state_buffer[3];
				
				state = 0x00;
				uart_send('F');
			}
			
		// display shift up
		} else if (state == 'G') { 
			if (state_position >= 4) {
				display_shift_up();
				buffer[DISPLAY_BUFFER_LAST_LINE + 0] = state_buffer[0];
				buffer[DISPLAY_BUFFER_LAST_LINE + 1] = state_buffer[1];
				buffer[DISPLAY_BUFFER_LAST_LINE + 2] = state_buffer[2];
				buffer[DISPLAY_BUFFER_LAST_LINE + 3] = state_buffer[3];
				
				state = 0x00;
				uart_send('G');
			}
			
		// invalid state -> reset
		} else { 
			uint8_t temp = state;
			state = 0x00;
			uart_send(temp);
		}
    }
	
    return (0);
}