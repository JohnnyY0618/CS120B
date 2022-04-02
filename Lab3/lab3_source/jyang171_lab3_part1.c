/*	Author: Jianeng Yang
 *  Partner(s) Name: none
 *	Lab Section:21
 *	Assignment: Lab #3 Exercise #1
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
	DDRB = 0X00; PORTB = 0XFF; // Configure port B's 8 pins as inputs
	DDRC = 0XFF; PORTC = 0X00; // Configure port C's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00; // Temporary variable to hold the value of C
    /* Insert your solution below */
	while (1) {
		tmpA = PINA; // assign to hold value of A
		tmpB = PINB; // assign to hold value of B
		tmpC = 0x00;
		// check the number of 1s in port A
		for (int i = 0; i <= 7; i++) {
			if ( (tmpA & 0x01) == 0x01 ) { // if PA0 = 1,PA1 = 1, ...PA7 = 1
				tmpC = tmpC + 1; // increment of count of # 1s
			}
			tmpA = tmpA >> 1; // shift 1 bit to right, ex:PA7PA6PA5PA4PA3PA2PA1PA0 -> 0PA7PA6PA5PA4PA3PA2PA1
		}

		// check the number of 1s in port B
		for (int i = 0; i <= 7; i++) {
			if ( (tmpB & 0x01) == 0x01 ) { // if PB0 = 1,PB1 = 1, ...PB7 = 1
				tmpC = tmpC + 1; // increment of count of # 1s
			}
			tmpB = tmpB >> 1; // shift 1 bit to right, ex:PB7PB6PB5PB4PB3PB2PB1PB0 -> 0PB7PB6PB5PABPB3PB2PB1
		}
		
		// write output
		PORTC = tmpC;
	}
	return 1;
}
