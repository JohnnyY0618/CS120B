/*	Author: Jianeng Yang
 *  Partner(s) Name: None
 *	Lab Section: 21
 *	Assignment: Lab # 2  Exercise # 4
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
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs,
	DDRC = 0x00; PORTC = 0xFF; // Configure port c's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00; // Temporary variable to hold the value of C
	unsigned char tmpD = 0x00; // Temporary variable to hold the value of D
	unsigned short totalWeight = 0x0000; // variable to hold the sum of value of A,B,C
	unsigned char finalPortD = 0x00; // store the value that will assign to port D


    /* Insert your solution below */
	while (1) {
		tmpA = PINA; // Temparary variable to hold the value(weight) of A
		tmpB = PINB; // Temparary variable to hold the value(weight) of B
		tmpC = PINC; // Temparary variable to hold the value(weight) of C
		tmpD = 0x00;
		// 2) Perform computation
		// 
		totalWeight = tmpA + tmpB + tmpC; // sum of weight detected by A, B ,C
		if (totalWeight > 0x8C) { // true if totalWeight is greater than 140 lbs(0x8C)
			tmpD = 0x01; // set PD0 to 1		
		}
		else {
			tmpD = 0x00; // set PD0 to 0
		}
		
		// check the different value between A and C is exceed 80 or not
		if (tmpA > tmpC) { // check if A is large than C
			if ( (tmpA - tmpC) > 0x50) { // and if is large than 80 lbs(0x50)
				tmpD = tmpD | 0x02; // set PD1 = 1; 
			}
		}
		else { // check C large than A
			if ((tmpC - tmpA) > 0x50) { // and if their differen is large than 80
				tmpD = tmpD | 0x02; // set PD1 = 1;	
			}
		}
		// write ouput: 
		finalPortD = ((totalWeight >> 2) & 0xFC) | tmpD;
		PORTD = finalPortD;
	}
	return 0;

}
