/*	Author: Jianeng Yang
 *  Partner(s) Name: none
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES {Start, Pressed, Released, Pressed2, Released2} STATE;

void pressed_Trick() {
	switch (STATE) {
		case Start:
			if ((PINA & 0x01) != 0x01) { // if !PA0
				STATE = Start; // state in state Start 
			}
			else {
				STATE = Pressed;
			}
			break;
		case Pressed:
			if ((PINA & 0x01) != 0x01) { // if !PA0
				STATE = Released;
			}
			else {
				STATE = Pressed;
			}
			break;
		case Released:
			if ((PINA & 0x01) != 0x01) { // if !PA0
				STATE = Released;
			}
			else {
				STATE = Pressed2;
			}
			break;
		case Pressed2:
			if ((PINA & 0x01) != 0x01) { // if !PA0
				STATE = Released2;
			}
			else {
				STATE = Pressed2;
			}
			break;

		case Released2:
			if ((PINA & 0x01) != 0x01) { // if !PA0
				STATE = Released2;
			}
			else {
				STATE = Pressed;
			}
			break;
		default:
			break;
	}
	switch (STATE) {
		case Start:
			PORTB = 0x01;
			break;
		case Pressed:
			PORTB = 0x02;
			break;	
		case Released:
			PORTB = 0x02;
			break;	
		case Pressed2:
			PORTB = 0x01;
			break;	
		case Released2:
			PORTB = 0x01;
			break;
		default:
			break;	
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0XFF; PORTB = 0X00;
    /* Insert your solution below */
    while (1) {
	pressed_Trick();
    }
    return 1;
}
