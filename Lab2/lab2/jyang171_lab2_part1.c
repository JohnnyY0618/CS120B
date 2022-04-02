/*	Author: Jianeng Yang
 *  Partner(s) Name: None
 *	Lab Section: 21
 *	Assignment: Lab # 2  Exercise # 1
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
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A

    /* Insert your solution below */
	while (1) {
		tmpA = PINA & 0x01; //PA0 (1 means door open)
		tmpB = PINA & 0x02; //PA1 (1 means light is sensed).
		// 2) Perform computation
		// if PA0 is 1, set PB0 = 1, else = 0
		if (tmpA == 0x01 && tmpB == 0x00) { // True if PA0 is 1 and PA1 is 0
			PORTB = 0x01; // Sets PORTB to 00000001, PB0 = 1; 
		} else {
			PORTB = 0x00; // Sets PORTB to 00000000, PB0 = 0;
		}		
	}
	return 0;

}
