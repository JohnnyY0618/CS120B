/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #6  Exercise #2
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

enum Led_States{Start, Begin, Next, Next1, Pause, Pause1} Led_State;

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

void Led_Tick() {

	unsigned char P_A0 = 0x00; //for store PA0, button pressed
	P_A0 = ~PINA & 0x01; // store PA0;
	
	// For pause and unpase purpose
	unsigned char P_B0 = 0x00;
	unsigned char P_B1 = 0x00;
	unsigned char P_B2 = 0x00;
	P_B0 = PORTB & 0x01;
	P_B1 = PORTB & 0x02;
	P_B2 = PORTB & 0x04;

	switch (Led_State) { // transitions
		case Start:
			Led_State = Begin;
			break;
		case Begin:
			if (P_A0) {
				Led_State = Pause;			
			}
			else {
				Led_State = Next;			
			}
			break;
		case Next:
			if (P_A0) {
				Led_State = Pause;			
			}
			else {
				Led_State = Next1;			
			}
			break;
		case Next1:
			if (P_A0) {
				Led_State = Pause;			
			}
			else {
				Led_State = Begin;			
			}
			break;
		case Pause:
			Led_State = Pause1;
			break;
		case Pause1:
			if (P_A0) {
				if (P_B0) {
					Led_State = Next;
				}
				else if (P_B1) {
					Led_State = Next1;
				}
				else if (P_B2) {
					Led_State = Begin;
				}
			}
			else {
				Led_State = Pause1;
			}
			break;
		default:
			Led_State = Start;
			break;
	}
	switch (Led_State) { // actions
		case Start:
			break;
		case Begin:
			PORTB = 0x01; // first led light on
			break;
		case Next:
			PORTB = 0X02;
			break;
		case Next1:
			PORTB = 0X04;
			break;
		case Pause:
			break;
		case Pause1:
			break;
	}
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // for input PROTA
	DDRB = 0xFF; PORTB = 0x00; // for output PORTB
    /* Insert your solution below */
	// set timer to 300ms
	TimerSet(300); 
	// timer is on
	TimerOn();
    while (1) {
	Led_Tick();
	while(!TimerFlag) {} // wait 1 sec
	TimerFlag = 0;
    }
    return 1;
}
