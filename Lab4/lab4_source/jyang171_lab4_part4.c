

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

enum Button_States {Start, Init, pressPound, pressY, releasePound, unlockRelease, Locked, pressPoundL,pressYL, releasePoundL, lockRelease} Button_State;


void pressed_Tick() {

	unsigned char P_A0 = 0x00;  // for store PA0, button X

	unsigned char P_A1 = 0x00;  // for store PA1, button Y

	unsigned char P_A2 = 0x00;  // for store PA2, button #

	unsigned char P_A7 = 0x00;  // for store PA2, button inside

	P_A0 = PINA & 0x01;  // PA0, button X

	P_A1 = PINA & 0x02;  // PA1, button Y

	P_A2 = PINA & 0x04;  // PA2, button #

	P_A7 = PINA & 0x80;  //   d5rrf 

	switch (Button_State) {
		case Start:
			Button_State = Init;
			break;

		case Init:
			if (P_A2 && !P_A1 && !P_A0) { // if # being presse (PA2 =1)
				Button_State = pressPound;
			}
			else if (!P_A2 && !P_A1 && !P_A0) { 
				Button_State = Init; 
			}
			else { // press inside button (PA7)
				Button_State = Locked; // lock the door
			}
			break;

		case pressPound:
			if (P_A2 && !P_A1 && !P_A0) {
				Button_State = pressPound;		
			}			
			else if (!P_A2 && !P_A1 && !P_A0){
				Button_State = releasePound;
			}
			else {
				Button_State = Locked; // lock the door			
			}
			break;

		case releasePound: 
			if (P_A1 && !P_A2 && !P_A0) {
				Button_State = pressY;			
			}
			else if (!P_A1 && !P_A2 && !P_A0) {
				Button_State = releasePound;
			}
			else {
				Button_State = Locked;
			}

			break;	

		case pressY:

			if (P_A1 && !P_A2 && !P_A0 && !P_A7) {

				Button_State = pressY;
			}
			else {
				Button_State = unlockRelease;
			}
			break;

		case unlockRelease:

			if (!P_A2 && !P_A1 && !P_A0 && !P_A7) {
				Button_State = unlockRelease;			
			}
			else if (!P_A2 && !P_A1 && !P_A0 && P_A7) {
				Button_State = Locked;			
			}
			else if (P_A2 && !P_A1 && !P_A0 && !P_A7) {
				Button_State = pressPoundL;
			}
			else {
				Button_State = Init;
			}
			break;

		case Locked:
			if (P_A7 && !P_A2 && !P_A1 && !P_A0) {
				Button_State = Locked;
			}
			else if (!P_A7 && !P_A2 && !P_A1 && !P_A0) {
				Button_State = unlockRelease;			
			}
			else {
				Button_State = Init;
			}

			break;

		case pressPoundL:
			if (P_A2 && !P_A1 && !P_A0) {
				Button_State = pressPoundL;		
			}			
			else if (!P_A2 && !P_A1 && !P_A0){
				Button_State = releasePoundL;
			}
			else {
				Button_State = Locked; // lock the door			
			}
			break;

		case releasePoundL: 
			if (P_A1 && !P_A2 && !P_A0) {
				Button_State = pressYL;			
			}
			else if (!P_A1 && !P_A2 && !P_A0) {
				Button_State = releasePoundL;
			}
			else {
				Button_State = Locked;
			}

			break;	

		case pressYL:

			if (P_A1 && !P_A2 && !P_A0 && !P_A7) {

				Button_State = pressYL;
			}
			else {

				Button_State = lockRelease;

			}

			break;

		case lockRelease:

			if (!P_A2 && !P_A1 && !P_A0 && !P_A7) {
				Button_State = lockRelease;			
			}
			else if (!P_A2 && !P_A1 && !P_A0 && P_A7) {
				Button_State = Locked;			
			}
			else if (P_A2 && !P_A1 && !P_A0 && !P_A7) {
				Button_State = pressPound;
			}
			else {
				Button_State = Init;
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

		case pressPound:			

			break;

		case releasePound:

			break;

		case pressY:

			break;

		case unlockRelease:

			PORTB = 0x01;			

			break;

		case Locked:

			PORTB = 0x00;

			break;	
		case lockRelease:

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
