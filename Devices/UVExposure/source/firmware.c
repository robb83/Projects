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
	0b00010001, 0b11010111, 0b00110010, 0b10010010, 0b11010100,
	0b10011000, 0b00011000, 0b11010011, 0b00010000, 0b10010000,
};
uint8_t button_state[] = {
	0, 0, 0, 0
};

void timer_stoped() {
	// switch off uv led, switch on red led
	
	PORTB |= (_BV(PB4));
	PORTB &= ~_BV(PB3);
}

void timer_started() {
	// switch on uv led, switch off red led
	
	PORTB &= ~_BV(PB4);
	PORTB |= (_BV(PB3));
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
	
	button_state[BUTTON1] = bit_is_clear(current, PD2);
	button_state[BUTTON2] = bit_is_clear(current, PD3);
	button_state[BUTTON3] = bit_is_clear(current, PD4);
	button_state[BUTTON4] = bit_is_clear(current, PD5);
}

void button_handler() {
	if (button_state[BUTTON1]) {
		timer_down();
		
		_delay_ms(150);
	}
	
	if (button_state[BUTTON2]) {
		timer_up();
		
		_delay_ms(150);
	}
	
	if (button_state[BUTTON3]) {
		timer_write();
		
		_delay_ms(250);
	}
	
	if (button_state[BUTTON4]) {
		timer_toggle();
		
		_delay_ms(250);
	}
}

void push_single_digit(uint8_t digit) {
	uint8_t i, v;

	v = int_to_char_mapping[digit % 10];
	
	for(i=0; i < 8; i++) {
		
		if (bit_is_clear(v, i)) {
			PORTB &= ~_BV(PB0);
		} else {
			PORTB |= _BV(PB0);
		}
		
		//SRCLK
		cbi(PORTB, PB2);
		sbi(PORTB, PB2);
	}
}

void display_integer(uint16_t value) {
	
	value = CLAMP(value, 0, 999);
	push_single_digit((value / 100));
	push_single_digit((value % 100) / 10);
	push_single_digit((value % 10));
	
	// RCLK
	cbi(PORTA, PA0);
	sbi(PORTA, PA0);
}

void display_reset() {
	PORTA &= ~_BV(PA1);
	PORTA |= _BV(PA1);
}

void heartbeat() {
	PORTD ^= _BV(PD6);
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
	// heartbeat led: output
	DDRD |= _BV(DDD6);
	
	// uv & red led: output
	PORTB &= ~(_BV(PB4) | _BV(PB3));
	DDRB |= (_BV(DDB4) | _BV(DDB3));
	
	// button: internal pullup & input
	PORTD |= (_BV(PD5) | _BV(PD4) | _BV(PD3) | _BV(PD2));
	DDRD &= ~(_BV(DDD5) | _BV(DDD4) | _BV(DDD3) | _BV(DDD2));
	
	// shift register ports
	DDRB |= (_BV(DDB0) | _BV(DDB1) | _BV(DDB2));
	DDRA |= (_BV(DDRA0) | _BV(DDRA1));

	// setup timer0
	TCNT0 = 0x00; // counter value
	TCCR0B = 0x03; // 64 divider
	TIMSK |= _BV(TOIE0); // enable interrupt

	// enable global interrupts
	sei();
	
	// reset display
	display_reset();
	
	// read timer settings
	timer_read();

	while(1) {
		// read button state
		button_update();
		
		// handle buttons
		button_handler();

		// display current time
		display_integer(time_remaning);
	}
}
