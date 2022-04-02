

/*	Author: Jianeng Yang
 *  Partner(s) Name: none
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Button_States {Start, Init, Correct, Wrong, Unlock, Locked} Button_State;

unsigned char index = 0x00;
unsigned char unlock_Codes[4] = {0x04, 0x01,0x02,0x01}; //press #-X-Y-X

void pressed_Tick() {

	unsigned char P_A0 = 0x00;  // for store PA0, button X
	unsigned char P_A1 = 0x00;  // for store PA1, button Y
	unsigned char P_A2 = 0x00;  // for store PA2, button #
	unsigned char P_A7 = 0x00;  // for store PA2, button inside

	P_A0 = PINA & 0x01;  // PA0, button X
	P_A1 = PINA & 0x02;  // PA1, button Y
	P_A2 = PINA & 0x04;  // PA2, button #
	P_A7 = PINA & 0x80;  // PA7, for button inside
	switch (Button_State) {
		case Start:
			Button_State = Init;
			break;

		case Init:
			if ((PINA & 0x87) == unlock_Codes[index]) { // compare our input to the array codes
				Button_State = Correct;
			}
			else if (P_A7 && !P_A2 && !P_A1 && !P_A0) { // if is A7, (PINA & 0x87) == 0x80
				Button_State = Locked;			
			}
			else if (!P_A7 && !P_A2 && !P_A1 && !P_A0) { // if all 4 button no being press, PINA == 0x00
				Button_State = Init;
							
			}
			else {
				Button_State = Wrong;
			}
			break;

		case Correct:
			if ( (index == 0x03) && ((PORTB & 0x01) == 0x01) ) {
				Button_State = Locked;		
			}			
			else if (index == 0x03) {
				Button_State = Unlock;
			}
			else if (!P_A7) {
				Button_State = Init;
				index++;
			}
			else {
				Button_State = Locked; // lock the door			
			}
			break;

		case Wrong: 
			if (!P_A2 && !P_A1 && !P_A0) { // if not press all 3 button
				Button_State = Init;
				index = 0x00;			
			}
			else {
				Button_State = Locked;
			}

			break;	

		case Unlock:
			if (!P_A2 && !P_A1 && !P_A0) {
				Button_State = Init;
			}
			else {
				Button_State = Unlock;
			}
			break;

		case Locked:
			if (P_A7 && !P_A2 && !P_A1 && !P_A0) {
				Button_State = Locked;
			}
			else {
				Button_State = Init;
				index = 0x00;
			}


			break;

		default:

			Button_State = Start;

			break;

	}

	switch (Button_State) {

		case Start:

			PORTB = 0x00;

			break;

		case Init:

			break;

		case Correct:			

			break;

		case Unlock:
			PORTB = 0x01;
			break;

		case Locked:

			PORTB = 0x00;

			break;				
		default:

			break;	

	}


}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0XFF; PORTC = 0X00;
    /* Insert your solution below */
    while (1) {
	pressed_Tick();
    }
    return 1;
}
