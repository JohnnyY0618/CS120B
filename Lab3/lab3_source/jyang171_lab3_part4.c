/*	Author: Jianeng Yang
 *  Partner(s) Name: none
 *	Lab Section:21
 *	Assignment: Lab #3 Exercise #4
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
	DDRB = 0XFF; PORTB = 0X00; // Configure port B's 8 pins as outputs,initialize to 0s
	DDRC = 0XFF; PORTC = 0X00; // Configure port C's 8 pins as outputs,initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of C
	unsigned char tmpC = 0x00; // Temporary variable to hold the value of C
    /* Insert your solution below */
	while (1) {
		tmpA = PINA; // assign to hold value of A
		tmpB = tmpA >> 4; // PB7PB6PB5PB4 PB3PB2PB1PB0 = 0000 PA7PA6PA5PA4
		tmpC = tmpA << 4; // PC7PC6PC5PC4 PC3PC2PC1PC0 = PA3PA2PA1PA0 0000
		
		
		// write output
		PORTB = tmpB;
		PORTC = tmpC;
	}
	return 1;
}
