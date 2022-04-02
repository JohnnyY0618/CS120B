/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #7  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/drive/folders/1-7c2dC4yUD4R__-nomgj7kU03B7jLK9f?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short x = 0x00;
// value of ADC register will stored in variable x.

// for exericen number 2: found out the MAX value can be 8 led light up, SO MAX = 0xFF;
unsigned short MAX_VALUE = 0xFF;

// light pattern
unsigned char lights[8] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1<< ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // input for potentiometer 
    	DDRB = 0xFF; PORTB = 0x00; // output
    	DDRC = 0xFF; PORTC = 0x00; // output 
    	DDRD = 0xFF; PORTD = 0x00; // output
    /* Insert your solution below */
	
    // call ADC initialize
    ADC_init();
    while (1) {
	x = ADC;
	unsigned char X_in_B = (char)x;
	unsigned average = MAX_VALUE/8;
	//PORTB= (char)x;  // = DC , lower 8 bits on port B
	//PORTD= (char)(x>>8); //  upper 2 bits on port D

	if ( X_in_B <= average) {
		PORTB = lights[0];
	}
	else if ( X_in_B <= (average*2)) {
		PORTB = lights[1];
	}
	else if ( X_in_B <= (average*3)) {
		PORTB = lights[2];
	}
	else if ( X_in_B <= (average*4)) {
		PORTB = lights[3];
	}
	else if ( X_in_B <= (average*5)) {
		PORTB = lights[4];
	}
	else if ( X_in_B <= (average*6)) {
		PORTB = lights[5];
	}
	else if ( X_in_B <= (average*7)) {
		PORTB = lights[6];
	}
	else if ( X_in_B <= (MAX_VALUE)) {
		PORTB = lights[7];
	}
    }
    return 1;
}
