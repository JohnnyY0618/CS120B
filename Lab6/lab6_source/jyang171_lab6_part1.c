/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #6  Exercise #1
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

enum Led_States{Start, Begin, Next, Next1} Led_State;

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
	switch (Led_State) { // transitions
		case Start:
			Led_State = Begin;
			break;
		case Begin:
			Led_State = Next;
			break;
		case Next:
			Led_State = Next1;
			break;
		case Next1:
			Led_State = Begin;
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
	}
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00; // output PORTB
    /* Insert your solution below */
	// set timer to 1 sec = 1000ms
	TimerSet(1000);
	// timer is on
	TimerOn();
    while (1) {
	Led_Tick();
	while(!TimerFlag) {} // wait 1 sec
	TimerFlag = 0;
    }
    return 1;
}
