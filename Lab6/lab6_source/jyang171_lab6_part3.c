/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *   Demo Link: https://drive.google.com/drive/folders/1-40xR8TUF4cZXBkIzP_aVsbSzQcs8eWv?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() { 
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum Button_States {Start, Begin, PressedA0, incA0, PressedA1, decA1, Reset} Button_State;

void Led_Tick() {
	unsigned char P_A0 = 0x00;  // for store PA0
	unsigned char P_A1 = 0x00;  // for store PA1
	P_A0 = (~PINA & 0x01);
	P_A1 = (~PINA & 0x02);
	switch (Button_State) {
		case Start:
			if (1) {
				PORTB = 0x07;
			}
			Button_State = Begin;
			break;
		case Begin:
			if (!P_A1 && P_A0 && (PORTB < 9) ) { // if PA0 && !PA1
				PORTB = PORTB + 1;
				Button_State = incA0;
			}
			else if (P_A1 && !P_A0 && (PORTB > 0) ) { // if !PA0 && PA1
				PORTB = PORTB - 1;
				Button_State = decA1;
			}
			else if (P_A1 && P_A0) { // if PA0 && PA1
				Button_State = Reset;
			}
			else {
				Button_State = Begin;
			}
			break;
		case PressedA0:
			if (P_A1 && P_A0) {
				Button_State = Reset;			
			}
			else {
				Button_State = Begin;
			}
			break;
		case PressedA1: 
			if (P_A1 && P_A0) {
				Button_State = Reset;			
			}
			else {
				Button_State = Begin;
			}
			break;	
		case incA0:
			Button_State = PressedA0;
			break;
		case decA1:			
			Button_State = PressedA1;
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
			PORTB = 0x07;
			break;
		case Begin:
			break;
		case PressedA0:			
			break;
		case PressedA1:			
			break;
		case incA0:
			break;
		case decA1:
			break;
		case Reset:
			PORTB = 0x00;
			break;	
		default:
			break;	
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // for input PROTA
	DDRB = 0xFF; PORTB = 0x00; // for output PORTB
    /* Insert your solution below */
	// set timer to 100ms
	TimerSet(100); 
	// timer is on
	TimerOn();
    while (1) {
	Led_Tick();
	while(!TimerFlag) {} // wait 100 ms
	TimerFlag = 0;
    }
    return 1;
}
