// reference
// http://ucexperiment.wordpress.com/2012/03/16/examination-of-the-arduino-millis-function/
// http://www.electrobob.com/uv-exposure-box-part-1-the-box/

// notes
// BUTTON1: timer counter decrement
// BUTTON2: timer counter increment
// BUTTON3: store current timer counter in eeprom
// BUTTON4: Start/Stop timer (if timer stoped and counter equal to zero then reload from eeprom)

#define CLAMP(x, min, max) (x < min ? min : ( x > max ? max : x))

#define F_CPU 8000000UL
#define EEPROM_TIME_REMANING_ADDRESS 0x04

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#define STATE_PAUSED 0
#define STATE_STARTED 1
#define STATE_UP 0
#define STATE_DOWN 1
#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define BUTTON4 3

volatile uint8_t time_fract = 0;     // 0 - 130
volatile uint16_t time_millisec = 0; // 0 - 1001
volatile int16_t time_remaning = 0;  // 0 - 999 (hopefully)
volatile uint8_t timer_state = 0;

uint8_t int_to_char_mapping[] = {
	0b10000001, 0b11101101, 0b01000011, 0b01001001, 0b00101101,
	0b00011001, 0b00010001, 0b11001101, 0b00000001, 0b00001001,
};
uint8_t button_state[] = {
	0, 0, 0, 0
};
uint16_t skipcycle = 0; // debounce effect

void timer_stoped() {
	// switch off uv led, switch on red led
	
	PORTD |= (_BV(PD0));
	PORTD &= ~_BV(PD1);
}

void timer_started() {
	// switch on uv led, switch off red led
	
	PORTD &= ~_BV(PD0);
	PORTD |= (_BV(PD1));
}

void timer_up() {
	time_remaning = (time_remaning + 1) % 1000;
}

void timer_down() {
	if (timer_state == STATE_STARTED && time_remaning == 0) {
		timer_state = STATE_PAUSED;
		
		timer_stoped();
	} else {
		time_remaning = (1000 + (time_remaning - 1)) % 1000;
	}
}

void timer_tick() {
	if (timer_state == STATE_STARTED) {
		timer_down();
	}
}

void timer_read() {
	time_remaning = (int16_t)eeprom_read_word((uint16_t*)EEPROM_TIME_REMANING_ADDRESS);
	time_remaning = CLAMP(time_remaning, 0, 999);
	
	timer_state = STATE_PAUSED;
	timer_stoped();
}

void timer_write() {
	eeprom_update_word((uint16_t*)EEPROM_TIME_REMANING_ADDRESS, time_remaning);
}

void timer_toggle() {
	if (timer_state == STATE_PAUSED) {
		if (time_remaning == 0) {
			timer_read();
			return;	
		}
		timer_state = STATE_STARTED;
		timer_started();
	} else {
		timer_state = STATE_PAUSED;
		timer_stoped();
	}
}

void button_update() {
	uint8_t current = PIND;
	
	button_state[BUTTON1] = bit_is_clear(current, PD6);
	button_state[BUTTON2] = bit_is_clear(current, PD5);
	button_state[BUTTON3] = bit_is_clear(current, PD4);
	button_state[BUTTON4] = bit_is_clear(current, PD3);
}

void button_handler() {
	if (button_state[BUTTON1]) {
		timer_down();
		
		skipcycle = 500;
	}
	
	if (button_state[BUTTON2]) {
		timer_up();
		
		skipcycle = 500;
	}
	
	if (button_state[BUTTON3]) {
		timer_write();
		
		skipcycle = 1500;
	}
	
	if (button_state[BUTTON4]) {
		timer_toggle();
		
		skipcycle = 1500;
	}
}

void display_integer(uint16_t value) {
	value = CLAMP(value, 0, 999);
	
	uint8_t d1 = (value / 100) % 10;
	uint8_t d2 = (value / 10) % 10;
	uint8_t d3 = value % 10;

	PORTB = int_to_char_mapping[d1];
	PORTA |= _BV(PA0);
	_delay_us(200);
	PORTA &= ~_BV(PA0);
	
	PORTB = int_to_char_mapping[d2];
	PORTA |= _BV(PA1);
	_delay_us(200);
	PORTA &= ~_BV(PA1);
	
	PORTB = int_to_char_mapping[d3];
	PORTD |= _BV(PD2);
	_delay_us(200);
	PORTD &= ~_BV(PD2);
}

void heartbeat() {

}

ISR(TIMER0_OVF_vect) {
	time_fract += 6;
	time_millisec += 2;
	
	if (time_fract >= 125) {
		time_fract -= 125;
		time_millisec += 1;
	}
	
	if (time_millisec >= 1000) {
		time_millisec -= 1000;
		
		timer_tick();
		heartbeat();
	}
}

int main (void) {
	// port configuration
	PORTD = 0b01111011;
	PORTB = 0b00000000;
	DDRA = 0b11111111;
	DDRD = 0b00000111;
	DDRB = 0b11111111;

	// setup timer0
	TCNT0 = 0x00; // counter value
	TCCR0B = 0x03; // 64 divider
	TIMSK |= _BV(TOIE0); // enable interrupt

	// enable global interrupts
	sei();
	
	// read timer settings
	timer_read();

	while(1) {
		if (skipcycle == 0) {
			// read button state
			button_update();
		
			// handle buttons
			button_handler();
		} else {
			--skipcycle;
		}

		// display current time
		display_integer(time_remaning);
	}
}
