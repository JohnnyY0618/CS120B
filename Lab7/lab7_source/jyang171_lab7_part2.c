/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *                            MAX VALUE: 0xFF, b/c by obserbation, it can be light up 8 led lights
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
	PORTB= (char)x;  // = DC , lower 8 bits on port B
	PORTD= (char)(x>>8); //  upper 2 bits on port D
    }
    return 1;
}
