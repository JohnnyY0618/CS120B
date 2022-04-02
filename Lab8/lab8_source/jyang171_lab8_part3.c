/*	Author: Jianeng Yang
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #3
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

// array store frequency: C A# A F G G D C A# A A A C A# A G G  A#- A- A#- A- A#- G G
//double tones[25] = {261.63,466.16,440.00,349.23,392.00,392.00,293.66,261.63,466.16,440.00,440.00,440.00,261.63,466.16,440.00,392.00,392.00,466.16,440.00,466.16};
//int times[20] ={5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

// array store frequency: Em C G D  Em C D Am Em Em C G D Em C D B       Em C G D Em C D Am Em
double tones[17] = {329.63,261.63,392.00,293.66,329.63,523.25,293.66,440.00,329.63,329.63,261.63,392.00,293.66,329.63,261.63,293.66,493.88};
// array store each frequncey play time range
int times[17] = {4,1,3,2,1,1,3,1,1,3,5,2,8,4,6,2,4};
unsigned char isEnd = 0; // 0 indicate false, we assume our melody is no end
unsigned char index = 0; // store the currect tones of index
unsigned char play_time = 0;
enum Button_States {Start, Begin,playOn,Release} Button_State;

void Sound_Tick() {
	unsigned char P_A0 = 0x00; // for stre PA0
	//unsigned char P_A1 = 0x00; // for stre PA1
	//unsigned char P_A2 = 0x00; // for stre PA2
	P_A0 = ~PINA & 0x01;
	//P_A1 = ~PINA & 0x02;
	//P_A2 = ~PINA & 0x04;

	switch (Button_State) { // transition
		case Start:
			Button_State = Begin;
			break;
		case Begin:
			if (P_A0) {
				Button_State = playOn;
			}
			else {
				Button_State = Begin;
			}
			break;
		case playOn:
			if (isEnd) {
				Button_State = Release;
			}
			else {
				Button_State = playOn;			
			}
			break;
		case Release:
			if (!P_A0) {
				Button_State = Begin;
			}
			else {
				Button_State = Release;			
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
			set_PWM(0);
			index = 0; // reset melody start at beginning
			isEnd = 0;
			break;
		case playOn:
			if (index >= 17) { // if our melody reach last tones
				isEnd = 1;  // is end of melody
			}
			else {
				if (play_time <= times[index]) {
					set_PWM(tones[index]);
					play_time++;
				}
				else {
					play_time = 0;
					index++;		
				}
			}
			break;
		case Release:
			set_PWM(0);
			index = 0; // reset melody start at beginning
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
