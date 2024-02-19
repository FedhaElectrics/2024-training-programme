/*
 * main.c
 *
 * Created: 2/15/2024 11:32:14 AM
 *  Author: calvin
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU 16000000L
#define LCD_PORT PORTD   // Port connected to the data pins of LCD
#define RS_PIN    PD2    // Register Select pin
#define EN_PIN    PD3    // Enable pin
#define REED_PIN   PB5   // Reed switch pin

volatile uint32_t pulseCount = 0;
volatile uint8_t magnetDetectedFlag = 0;
volatile uint8_t timeExpiredFlag = 0;

void LCD_command(unsigned char cmnd);
void LCD_data(unsigned char data);
void LCD_init();
void LCD_clear();
void LCD_setCursor(uint8_t row, uint8_t col);
void LCD_displayString(const char *str);

ISR (TIMER1_OVF_vect)
{
	TCNT1 = 16383;   // for 1 sec, counter 1,, 16 bit timer,, 16mhz
	timeExpiredFlag = 1;
}

ISR(PCINT0_vect)
{
	if (PINB & (1 << PINB5) && timeExpiredFlag)
	{
		
		pulseCount ++;
		magnetDetectedFlag = 1;
		timeExpiredFlag = 0;
	}
	
}


int main(void)
{
	DDRD = 0xFF;
	DDRB &= ~(1 << DDB5);
	TCNT1 = 16383;   // for 1 sec at 16 MHz
	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	PCMSK0 |= (1 << PCINT5);
	PCICR |= (1 << PCIE0);
	LCD_init();
	sei();

	while (1)
	{
		if (magnetDetectedFlag && !timeExpiredFlag)
		{
			LCD_clear();
			float count = pulseCount * 0.001; // Conversion factor: 1 count = 0.001 units
			char countStr[16];
			snprintf(countStr, sizeof(countStr), "Reading: %.3f", count);
			LCD_displayString(countStr);
			magnetDetectedFlag = 0;  // Clear the flag
			_delay_ms(200);  // Delay for smoother display
		}
	}
	return 0;
}

void LCD_command(unsigned char cmnd) {
	LCD_PORT = (LCD_PORT & 0x0F) | (cmnd & 0xF0);  // Send higher nibble
	PORTD &= ~(1 << RS_PIN);  // RS = 0 for command
	PORTD |= (1 << EN_PIN);   // EN = 1
	_delay_us(1);             // Delay
	PORTD &= ~(1 << EN_PIN);   // EN = 0

	_delay_us(200);           // Delay

	LCD_PORT = (LCD_PORT & 0x0F) | ((cmnd & 0x0F) << 4);  // Send lower nibble
	PORTD |= (1 << EN_PIN);   // EN = 1
	_delay_us(1);             // Delay
	PORTD &= ~(1 << EN_PIN);   // EN = 0

	_delay_ms(2);             // Delay
}

void LCD_data(unsigned char data) {
	LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);  // Send higher nibble
	PORTD |= (1 << RS_PIN);   // RS = 1 for data
	PORTD |= (1 << EN_PIN);   // EN = 1
	_delay_us(1);             // Delay
	PORTD &= ~(1 << EN_PIN);   // EN = 0

	_delay_us(200);           // Delay

	LCD_PORT = (LCD_PORT & 0x0F) | ((data & 0x0F) << 4);  // Send lower nibble
	PORTD |= (1 << EN_PIN);   // EN = 1
	_delay_us(1);             // Delay
	PORTD &= ~(1 << EN_PIN);   // EN = 0

	_delay_ms(2);             // Delay
}

void LCD_init() {
	_delay_ms(15);            // Wait for power-up

	LCD_command(0x02);        // Initialize in 4-bit mode
	LCD_command(0x28);        // 4-bit mode, 2-line, 5x8 font
	LCD_command(0x0E);        // Display ON, Cursor ON
	LCD_command(0x06);        // Increment cursor
	LCD_command(0x01);        // Clear display
	_delay_ms(2);             // Delay
}

void LCD_clear() {
	LCD_command(0x01);        // Clear display
	_delay_ms(2);             // Delay
}

void LCD_setCursor(uint8_t row, uint8_t col) {
	uint8_t position = (row == 1) ? (0x80 + col) : (0xC0 + col);
	LCD_command(position);
}

void LCD_displayString(const char *str) {
	while (*str != '\0') {
		LCD_data(*str);
		str++;
	}
}
