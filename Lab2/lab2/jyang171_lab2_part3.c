/*	Author: Jianeng Yang
 *  Partner(s) Name: None
 *	Lab Section: 21
 *	Assignment: Lab # 2  Exercise # 3
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
	
	unsigned char cntavail = 0 ;  // variable hold # aviable parking space and will assign to port C
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A


    /* Insert your solution below */
	while (1) {
		tmpA = PINA; // Temparary variable to hold the value of A
		// 2) Perform computation
		// Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space
		// if the 
		if (tmpA == 0x00) { // True if 0000 PA3PA2PA1PA = 0000,  4 parking availiable
			cntavail=4; // add 4 spaces 
		} 
		else if (tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08){ // check the 3 spaces cases
			cntavail=3; // add 3 spaces
		}
		else if (tmpA == 0x03 || tmpA == 0x05 || tmpA == 0x06 || tmpA == 0x09 || tmpA == 0x0A || tmpA == 0x0C){ // check the 2 spaces cases
			cntavail=2; // add 2 spaces
		}
		else if (tmpA == 0x07 || tmpA == 0x0B || tmpA == 0x0D || tmpA == 0x0E) { // check the 1 space case
			cntavail=1; // add 1 spaces
		}
		else if (tmpA == 0x0F) { // the parking space is full 0000 '1111'
			cntavail=0x80; // and set p7 to 1	
		}
		// write ouput: # of parking space is avaiablle
		PORTC = cntavail;	
	}
	return 0;

}
