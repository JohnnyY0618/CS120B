/*	Author: Jianeng Yang
 *  Partner(s) Name: none
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Button_States {Start, Begin, PressedA0, ReleasedA0, PressedA1, ReleasedA1, Reset} Button_State;

void pressed_Tick() {
	unsigned char P_A0 = 0x00;  // for store PA0
	unsigned char P_A1 = 0x00;  // for store PA1
	P_A0 = PINA & 0x01;
	P_A1 = PINA & 0x02;
	switch (Button_State) {
		case Start:
			if (1) {
				PORTC = 0x07;
			}
			Button_State = Begin;
			break;
		case Begin:
			if (!P_A1 && P_A0 && (PORTC < 9) ) { // if PA0 && !PA1
				PORTC = PORTC + 1;
				Button_State = PressedA0;
			}
			else if (P_A1 && !P_A0 && (PORTC > 0) ) { // if !PA0 && PA1
				PORTC = PORTC - 1;
				Button_State = PressedA1;
			}
			else if (P_A1 && P_A0) { // if PA0 && PA1
				Button_State = Reset;
			}
			else {
				Button_State = Begin;
			}
			break;
		case PressedA0:
			if (!P_A0) {
				Button_State = ReleasedA0;		
			}
			else if (P_A1 && P_A0) {
				Button_State = Reset;			
			}
			else {
				Button_State = PressedA0;
			}
			break;
		case PressedA1: 
			if (!P_A1) {
				Button_State = ReleasedA1;			
			}
			else if (P_A1 && P_A0) {
				Button_State = Reset;			
			}
			else {
				Button_State = PressedA1;
			}
			break;	
		case ReleasedA0:
			Button_State = Begin;
			break;
		case ReleasedA1:			
			Button_State = Begin;
			break;
		case Reset:
			if (P_A1 && P_A0) {
				Button_State = Reset;
			}
			else {
				Button_State = Begin;
			}
			break;
		default:
			Button_State = Start;
			break;
	}
	switch (Button_State) {
		case Start:
			PORTC = 0x07;
			break;
		case Begin:
			break;
		case PressedA0:			
			break;
		case PressedA1:			
			break;
		case ReleasedA0:
			break;
		case ReleasedA1:
			break;
		case Reset:
			PORTC = 0x00;
			break;	
		default:
			break;	
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0XFF; PORTC = 0X00;
    /* Insert your solution below */
    while (1) {
	pressed_Tick();
    }
    return 1;
}
