/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *   Demo Link: https://drive.google.com/drive/folders/1-9ab9aletNsVqYb_9KrtCBoov1ZFXlLi?usp=sharing
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

// counter
void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if(!frequency) {
            TCCR3B &= 0x08;
	}
        else {
            TCCR3B |= 0x03;
	}
        if(frequency < 0.954) {
            OCR3A = 0xFFFF;
	}
        else if (frequency > 31250) {
            OCR3A = 0x0000;
	}
        else {
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;
        }
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

// array store frequency C4 to C5
double tones[8] = {261.63,293.66,329.63,349.23,392.00,440.00,493.88,523.25};
unsigned char isOn = 0; // 0 indicate false, we assume the system is not on
unsigned char index = 0; // store the currect tones of index
enum Button_States {Start, Begin, pressedA0, pressedA1, release, OnOff} Button_State;

void Sound_Tick() {
	unsigned char P_A0 = 0x00; // for stre PA0
	unsigned char P_A1 = 0x00; // for stre PA1
	unsigned char P_A2 = 0x00; // for stre PA2
	P_A0 = ~PINA & 0x01;
	P_A1 = ~PINA & 0x02;
	P_A2 = ~PINA & 0x04;

	switch (Button_State) { // transition
		case Start:
			Button_State = Begin;
			break;
		case Begin:
			if (P_A0 && !P_A1 && !P_A2) {
				Button_State = pressedA0;
			}
			else if (P_A1 && !P_A0 && !P_A2) {
				Button_State = pressedA1;
			}
			else if (P_A2 && !P_A0 && !P_A1) {
				Button_State = OnOff;
			}
			else {
				Button_State = Begin;
			}
			break;
		case pressedA0:
			Button_State = release;
			break;
		case pressedA1:
			Button_State = release;
			break;
		case OnOff:
			Button_State = release;
			break;
		case release:
			if (!P_A0 && !P_A1 && !P_A2) {
				Button_State = Begin;
			}
			else {
				Button_State = release;
			}
			break;
		
		default:
			Button_State = Start;
			break;
	}
	switch (Button_State) {
		case Start:
			break;
		case Begin:
			break;
		case pressedA0:
			if (index < 7) { // if pattern not reach above C
				index++; // increse tone pattern 			
			}
			// if our system is on, play the incresent tones
			if (isOn) {
				set_PWM(tones[index]);			
			}
			break;
		case pressedA1:
			if (index > 0) { // if pattern not reach below c
				index--;  // decrease tone pattern			
			}
			// if our system is on, play the incresent tons
			if (isOn) {
				set_PWM(tones[index]);			
			}
			break;
		case OnOff:
			if (!isOn) { // if system is not on, then turn it on
				isOn = 1; // 2 indicate our system set to on
				set_PWM(tones[index]);
			}
			else { // if the system is on, then turn it off
				isOn = 0; // 0 indicate our system is off
				set_PWM(0); // turn off PWM
			}
			break;
		case release:
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
	PWM_on(); //Enables the ATmega1284’s PWM functionality. 
    while (1) {
	Sound_Tick();
	while(!TimerFlag) {} // wait 100 ms
	TimerFlag = 0;
    }
    return 1;
}
