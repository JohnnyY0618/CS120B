/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0X00; PORTA = 0XFF; // Configure port A's 8 pins as inputs
	//DDRB = 0X00; PORTB = 0XFF; // Configure port B's 8 pins as inputs
	DDRC = 0XFF; PORTC = 0X00; // Configure port C's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	//unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00; // Temporary variable to hold the value of C
    /* Insert your solution below */
	while (1) {
		tmpA = ~PINA; // assign to hold value of A
		tmpC = 0x00;
		
		// check fuel level 
		if ( (tmpA & 0x0F) == 1 || (tmpA & 0x0F) == 2 ) { // true if fuel level is 1 or 2
			tmpC = 0x20; // PC5 is light 0010 0000
		}
		else if ( (tmpA & 0x0F) == 3 || (tmpA & 0x0F) == 4 ) { // true if fuel level is 3 or 4
			tmpC = 0x30; // PC5, PC4 are light 0011 0000
		}
		else if ( (tmpA & 0x0F) >= 5 && (tmpA & 0x0F) <= 6 ) { // true if fuel level is 5-6
			tmpC = 0x38; // PC5, PC4 ,PC3 are light 0011 1000
		}
		else if ( (tmpA & 0x0F) >= 7 && (tmpA & 0x0F) <= 9 ) { // true if fuel level is 7-9
			tmpC = 0x3C; // PC5, PC4 ,PC3, PC2 are light 0011 1100
		}
		else if ( (tmpA & 0x0F) >= 10 && (tmpA & 0x0F) <= 12 ) { // true if fuel level is 10-12
			tmpC = 0x3E; // PC5, PC4 ,PC3 PC2, PC1 are light 0011 1110
		}
		else if ( (tmpA & 0x0F) >= 13 && (tmpA & 0x0F) <= 15 ) { // true if fuel level is 13-15
			tmpC = 0x3F; // PC5, PC4 ,PC3 PC2, PC1, PC0 are light 0011 1111
		}
		
		// check if is low fuel then PC6 is light
		if ( (tmpA & 0x0F) <= 4 ) { // true if fuel leve is 4 or less
			tmpC = tmpC | 0x40; // 0100 0000
		}
		// write output
		PORTC = tmpC;
	}
	return 1;
}
