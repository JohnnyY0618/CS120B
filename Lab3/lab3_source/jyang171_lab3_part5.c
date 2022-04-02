/*	Author: Jianeng Yang
 *  Partner(s) Name: none
 *	Lab Section:21
 *	Assignment: Lab #3 Exercise #5
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
	DDRD = 0X00; PORTD = 0XFF; // Configure port D's 8 pins as intputs
	DDRB = 0XFF; PORTB = 0X00; // Configure port B's 8 pins as outputs
	
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpD = 0x00; // Temporary variable to hold the value of D
	unsigned char finalB = 0x00; // variable to hold the final value of B
	unsigned short weight = 0x0000; // varbiable to hold teh passenger weight
    /* Insert your solution below */
	while (1) {
		tmpB = PINB; // assign to hold value of B
		tmpD = PIND; // assigne to hold value of D
		
		// weight = PD7...PD0PB0
		weight = (tmpD << 1) | (tmpB & 0x01);
		if (weight >= 70) { // true if equal or large than 70 lbs
			finalB = 0x02; // PB1 =1, meaning aribag be enabled
		}
		else if (weight > 5) { // true if less than 5 and less than 70 lbs
			finalB = 0x04; // PB2 =1, meaning airbad disabled
		}
		else { // PB1 and PB1 not be set
			finalB = 0x00;
		}

		// write output
		PORTB = finalB;
	}
	return 1;
}
