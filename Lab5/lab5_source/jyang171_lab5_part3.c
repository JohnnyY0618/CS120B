/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  	Demo Link: https://drive.google.com/file/d/1EJjctiNEnN9DpvA-rquJYym6DI-EAsBv/view?usp=sharing 
 */
#include <avr/io.h>
#include<stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

/*Define user variables and functions for this state machine here.*/
unsigned char i = 0x00;
unsigned char reverseP = 0x00; // 0 indicate false,we assume is our first path
unsigned char numPath = 0x08;
unsigned char lights[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};


enum Button_States {Start,Init,GoPath,Go,ReversePath,Reverse, ResetPath,Reset} Button_State;

void pressed_Tick() {
	unsigned char P_A0; // for store PA0
	P_A0 = ~PINA & 0x01;
	switch(Button_State) { // Transitions
		case Start:
			Button_State = Init;
			break;
		case Init:
			if ( !(i < numPath) || i < 0x00) {
				Button_State = ResetPath;
		
			}
			else if (P_A0 && !reverseP) { // if PA0 is presse and we are in first path
				Button_State = GoPath;
			}
			else if (P_A0 && reverseP) { // if PA0 is presse and we are in seconde path
				Button_State = ReversePath;
			}
			else {
				Button_State = Init;			
			}
			break;
		case ResetPath:
			Button_State = Reset;
			break;
		case GoPath:
			Button_State = Go;
			break;
		case ReversePath:
			Button_State = Reverse;
			break;

		case Reset:
			if (P_A0) {
				Button_State = Reset;			
			}
			else {
				Button_State = Start;
			}
			break;
		case Go:
			if (P_A0) {
				Button_State = Go;			
			}
			else {
				Button_State = Init;
			}
			break;
		case Reverse:
			if (P_A0) {
				Button_State = Reverse;			
			}
			else {
				Button_State = Start;
			}
			break;	
		default:
			Button_State = Start;
			break;	
		
	}
	switch (Button_State) {
		case Start:
			PORTC = 0X00;
			break;
		case Init:
			break;
		case ResetPath:
			if (!reverseP) {
				reverseP = 0x01;
				i = numPath-1; // make sure we start invert our bit in our last element				
			}
			else {
				reverseP = 0x00;
				i = 0x00; // make usre we start of fisrt path in first type of light			
			}	
			break;
		case GoPath:
			PORTC = lights[i];
			i++;
			break;
		case ReversePath:
			PORTC = ~lights[i];
			i--;
			break;
		case Reset:
			break;
		case Go:
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
