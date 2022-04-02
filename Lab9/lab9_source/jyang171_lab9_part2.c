/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/drive/folders/1T6rZZr-kZzxDxeT0eL_vrnU5d_vLI2_r?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// timer
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

// shored varibale
unsigned char threeLEDs = 0x00; 

// ThreeLEDsSM state machine
enum TLed_States {Led0,Led1,Led2} TLed_State;
void ThreeLEDsSM_Tick() {
	switch(TLed_State) {
		case Led0:
			TLed_State = Led1;
			break;	
		case Led1:
			TLed_State = Led2;
			break;
		case Led2:
			TLed_State = Led0;
			break;
		default:
			TLed_State = Led0;
			break;
	}
	switch(TLed_State) {
		case Led0:
			threeLEDs = threeLEDs & 0x08; // set three leds off,in case we can light only one led
			threeLEDs = threeLEDs | 0x01; // set bit 0 on
			break;	
		case Led1:
			threeLEDs = threeLEDs & 0x08; // set three leds off,in case we can light only one led
			threeLEDs = threeLEDs | 0x02; // set bit 1 on
			break;
		case Led2:
			threeLEDs = threeLEDs & 0x08; // set three leds off,in case we can light only one led
			threeLEDs = threeLEDs | 0x04; // set bit 2 on;
			break;
	}
}

// BlinkingLEDsSM state machine
enum BLed_States {LedOff, LedOn} BLed_State;
void BlinkingLEDsSM_Tick() {
	switch (BLed_State) {
		case LedOff:
			BLed_State = LedOn;
			break;
		case LedOn:
			BLed_State = LedOff;
			break;
		default:
			BLed_State = LedOff;
			break;
	}
	switch (BLed_State) {
		case LedOff:
			threeLEDs = threeLEDs & 0x07; // bit 3 off, keep the 0-2 bit
			break;
		case LedOn:
			threeLEDs = threeLEDs | 0x08; // bit 3 on
			break;
	}
}

// CombineLEDsSM state machine
enum CLed_States {Start, Light} CLed_State;
void CombineLEDsSM_Tick() {
	switch (CLed_State) {
		case Start:
			CLed_State = Light;
			break;
		case Light: 
			CLed_State = Light;
			break;
		default:
			CLed_State = Start;
			break;
	}

	switch (CLed_State) {
		case Start:
			break;
		case Light: 
			PORTB = threeLEDs;
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // for input PROTA
	DDRB = 0xFF; PORTB = 0x00; // for output PORTB
	// for have different time periods 
	unsigned long BL_elapsedTime = 1000;   // blinkingLEDs blinks 1 second on and off
   	unsigned long TL_elapsedTime = 300;    // threeLEDs light and change led in 300ms
   	const unsigned long timerPeriod = 100; 
    /* Insert your solution below */
	TimerSet(timerPeriod); // set to 100ms
	// timer is on
	TimerOn();
    while (1) {
	if (TL_elapsedTime >= 300) { // 300 ms period
		ThreeLEDsSM_Tick();  // Execute one tick
		TL_elapsedTime = 0;
	}
	if (BL_elapsedTime >= 1000) { // 1000 ms period(1 sec)
		BlinkingLEDsSM_Tick();	//Execute one tick
		BL_elapsedTime = 0;
	}
	CombineLEDsSM_Tick();
	while(!TimerFlag) {} // wait for timer period
	TimerFlag = 0;       // Lower flag raised by timer
	TL_elapsedTime += timerPeriod;
      	BL_elapsedTime += timerPeriod;

    }
    return 1;
}
