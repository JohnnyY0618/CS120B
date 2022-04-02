/*	Author: Jianeng Ynag
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #11  Final Project: Pong Game- "Advancement‌ ‌2 "
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *    Demo Link: https://drive.google.com/drive/folders/1tjcOQu_vLjnGwtyxQAROOGNEpr6yZS3e?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>  // for use rand()
#ifdef _SIMULATE_
#include "scheduler.h"
#include "timer.h"
#include "simAVRHeader.h"
#endif

//--------------------------------GLOBAL VARIABLE-----------------------------------//
// task 1 control the player 1 paddle
const unsigned char Player1_Pattern = 0x01;
unsigned char Player1_Row;
// task 3 control the player 2 paddle
const unsigned char Player2_Pattern = 0x80;
unsigned char Player2_Row;
// task for AI control the paddle
unsigned AI_Row;
const unsigned char AI_Pattern = 0x80;

// for check the way of ball bounce when it hit the paddle (player  1)
unsigned char upperPaddleRow;
unsigned char middlePaddleRow;
unsigned char lowerPaddleRow;

// for check the way of ball bounce when it hit the paddle AI
unsigned char upperAIPaddleRow;
unsigned char middleAIPaddleRow;
unsigned char lowerAIPaddleRow;

// for check the way of ball bounce when it hit the paddle (player 2)
unsigned char upperPaddleRow2;
unsigned char middlePaddleRow2;
unsigned char lowerPaddleRow2;

unsigned char isBeginAI = 0x00; // for check the game is begin or not
unsigned char isBeginP2 = 0x00; // for chekc the game is start with player 2

// for ball task
unsigned char Ball_Pattern = 0x08; // control ball column	
unsigned char Ball_Row = 0xFB;  // control ball row
unsigned char moveType = 0x02; // move left (1), move right(2), move left up(3), move left down(4), move right up(5), move right down(6), player1 lose round(7), player2/AI lose round(8)
unsigned char isReset = 0x00; // check for game to reset

// for keep track player's scores
unsigned char score1 = 0x00;
unsigned char score2 = 0x00;
unsigned char temp1 = 0x00;
unsigned char temp2 = 0x00;
unsigned char isSpin = 0x00;

//---------------------------------------------------------------------------------//

//--------------------------------ENUMs TICKFCT-----------------------------------//
enum Play1_States {P1_Begin,P1_Start,P1_init,P1_shiftRight,P1_shiftLeft, P1_releaseR,P1_releaseL};  // for task 1 control the player 1 paddle

//--------------------------------------------------------------------------------//

// task 1 for player 1 control the paddle
int Play1_Tick(int state) {
	// button for player 1 control the paddle
	unsigned char PB_0;
	unsigned char PB_1; 
	PB_0 = ~PINB & 0x01; // for button PB0
	PB_1 = ~PINB & 0x02; // For button PB1
	// Transitions
	switch (state) {
		case P1_Begin:
			if (isBeginAI || isBeginP2) {
				state = P1_Start;
			} 
			else {
				state = P1_Begin;
			}
			break;
		case P1_Start:
			state = P1_init;
			break;
		case P1_init:
			if (PB_0 && !PB_1) {
				state = P1_shiftRight;
			}
			else if (PB_1 && !PB_0){
				state = P1_shiftLeft;
			}
			else {
				state = P1_init;
			}
			break;
		case P1_shiftRight:
			state = P1_releaseR;
			break;
		case P1_releaseR:
			if (!PB_0 && !PB_1) {
				state = P1_init;
			}
			else {
				state = P1_releaseR;
			}
			break;
		case P1_shiftLeft:
			state = P1_releaseL;
			break;
		case P1_releaseL:
			if (!PB_1 && !PB_0) {
				state = P1_init;
			}
			else {
				state = P1_releaseL;
			}
			break;
		default:
			state = P1_Begin;
			break;
	}
	// Actions
	switch (state) {
		case P1_Begin:
			break;
		case P1_Start:
			Player1_Row = 0xF1;
			upperPaddleRow = 0xFD;
			middlePaddleRow = 0xFB;
			lowerPaddleRow = 0XF7;
			break;
		case P1_init:
			if (isReset) {
				Player1_Row = 0xF1;
				upperPaddleRow = 0xFD;
				middlePaddleRow = 0xFB;
				lowerPaddleRow = 0XF7;			
			}
			break;
		case P1_shiftRight:
			if (Ball_Pattern == 0x02 && middlePaddleRow == Ball_Row) {
				isSpin = 0x01;			
			}
			if (Player1_Row == 0xF1) {
				Player1_Row = 0xF8;
				upperPaddleRow = 0xFE;
				middlePaddleRow = 0xFD;
				lowerPaddleRow = 0xFB;
			}
			else if (Player1_Row == 0xE3) {
				Player1_Row = 0xF1;
				upperPaddleRow = 0xFD;
				middlePaddleRow = 0xFB;
				lowerPaddleRow = 0XF7;
			}
			break;
		case P1_releaseR:
			if (Ball_Pattern == 0x02 && middlePaddleRow == Ball_Row) {
				isSpin = 0x00;			
			}
			break;
		case P1_shiftLeft:
			if (Ball_Pattern == 0x02 && middlePaddleRow == Ball_Row) {
				isSpin = 0x02;			
			}
			if (Player1_Row == 0xF1) {
				Player1_Row = 0xE3;
				upperPaddleRow = 0xFB;
				middlePaddleRow = 0xF7;
				lowerPaddleRow = 0XEF;
			}
			else if (Player1_Row == 0xF8) {
				Player1_Row = 0xF1;
				upperPaddleRow = 0xFD;
				middlePaddleRow = 0xFB;
				lowerPaddleRow = 0XF7;
			}
			break;
		case P1_releaseL:
			if (Ball_Pattern == 0x02 && middlePaddleRow == Ball_Row) {
				isSpin = 0x00;			
			}
			break;
	}
	//PORTC = Player1_Pattern;	// Pattern to display
	//PORTD = Player1_Row;
	return state;
}


// task 4 for player 2 control the paddle
enum Play2_States {P2_Begin,P2_Start,P2_init,P2_shiftRight,P2_shiftLeft, P2_releaseR,P2_releaseL};  // for task 4 control the player 2 paddle
int Play2_Tick(int state) {
	unsigned char PA_0;
	unsigned char PA_1;
	PA_0 = ~PINA & 0x01; // for button PA0
	PA_1 = ~PINA & 0x02; // For button PA1
	// Transitions
	switch (state) {
		case P2_Begin:
			if (isBeginP2) {
				state = P2_Start;
			} 
			else {
				state = P2_Begin;
			}
			break;
		case P2_Start:
			state = P2_init;
			break;
		case P2_init:
			if (!isBeginP2) {
				state = P2_Begin;
			}
			else if (PA_0 && !PA_1) {
				state = P2_shiftRight;
			}
			else if (PA_1 && !PA_0){
				state = P2_shiftLeft;
			}
			else {
				state = P2_init;
				isSpin = 0x00;
			}
			break;
		case P2_shiftRight:
			state = P2_releaseR;
			break;
		case P2_releaseR:
			if (!PA_0) {
				state = P2_init;
			}
			else {
				state = P2_releaseR;
			}
			break;
		case P2_shiftLeft:
			state = P2_releaseL;
			break;
		case P2_releaseL:
			if (!PA_1) {
				state = P2_init;
			}
			else {
				state = P2_releaseL;
			}
			break;
		default:
			state = P2_Begin;
			break;
	}
	// Actions
	switch (state) {
		case P2_Begin:
			break;
		case P2_Start:
			Player2_Row = 0xF1;
			upperPaddleRow2 = 0xFD;
			middlePaddleRow2 = 0xFB;
			lowerPaddleRow2 = 0XF7;
			break;
		case P2_init:
			break;
		case P2_shiftRight:
			if (Player2_Row == 0xF1) {
				Player2_Row = 0xF8;
				upperPaddleRow2 = 0xFE;
				middlePaddleRow2 = 0xFD;
				lowerPaddleRow2 = 0xFB;
			}
			else if (Player2_Row == 0xE3) {
				Player2_Row = 0xF1;
				upperPaddleRow2 = 0xFD;
				middlePaddleRow2 = 0xFB;
				lowerPaddleRow2 = 0XF7;
			}
			break;
		case P2_releaseR:
			break;
		case P2_shiftLeft:
			if (Player2_Row == 0xF1) {
				Player2_Row = 0xE3;
				upperPaddleRow2 = 0xFB;
				middlePaddleRow2 = 0xF7;
				lowerPaddleRow2 = 0XEF;
			}
			else if (Player2_Row == 0xF8) {
				Player2_Row = 0xF1;
				upperPaddleRow2 = 0xFD;
				middlePaddleRow2 = 0xFB;
				lowerPaddleRow2 = 0XF7;
			}
			break;
		case P2_releaseL:
			break;
	}
	//PORTC = Player2_Pattern;	// Pattern to display
	//PORTD = Player2_Row;
	return state;
}

// 
int AI_Random = 0;
int num;
enum AI_States {AI_Begin, AI_Start,AI_init,AI_shiftUp,AI_shiftDown,AI_release};  // for task 2 control the AI paddle
// task 2 for AI paddle
int AI_Tick(int state) {
	// Transitions
	switch (state) {
		case AI_Begin:
			if (isBeginAI) {
				state = AI_Start;
			} 
			else {
				state = AI_Begin;
			}
			break;
		case AI_Start:
			state = AI_init;
			break;
		case AI_init:
			if (!isBeginAI) {
				state = AI_Begin;
			}
			else if (moveType == 0x03 && Ball_Pattern == 0x10) {
				if (num % 3 == 0) {
					state = AI_shiftDown;					
				}
				else {
					state = AI_shiftUp;
				}
				num++;			
			}
			else if (moveType == 0x04 && Ball_Pattern == 0x10){
				if (num % 4 == 0) {
					state = AI_shiftUp;					
				}
				else {
					state = AI_shiftDown;
						
				}
				num++;	
			}
			else if (moveType == 0x01 && Ball_Pattern == 0x10) {
				if ((num % 6) == 0) {
					state = AI_shiftDown;
				}
				else if ( (num % 5) == 0 ) {
					state = AI_shiftUp;
				}
				else {
					state = AI_init;
				}
				num++;
			}
			else {
				state = AI_init;
			}
			break;
		case AI_shiftUp:
			state = AI_release;
			break;
		case AI_shiftDown:
			state = AI_release;
			break;
		case AI_release:
			if (Ball_Pattern < 0x10) {
				state = AI_init;
			}
			else {
				state = AI_release;			
			}
			break;
		default:
			state = AI_Begin;
			break;
	}
	// Actions
	switch (state) {
		case AI_Begin:
			break;
		case AI_Start:
			AI_Row = 0xF1;
			upperAIPaddleRow = 0xFD;
			middleAIPaddleRow = 0xFB;
			lowerAIPaddleRow = 0XF7;
			break;
		case AI_init:
			if (isReset) {
				AI_Row = 0xF1;
				upperAIPaddleRow = 0xFD;
				middleAIPaddleRow = 0xFB;
				lowerAIPaddleRow = 0XF7;
				isReset = 0x00;
				num = 0;
			}
			break;
		case AI_shiftUp:
			if (AI_Row == 0xF1) {
				AI_Row = 0xF8;
				upperAIPaddleRow = 0xFE;
				middleAIPaddleRow = 0xFD;
				lowerAIPaddleRow = 0xFB;
			}
			else if (AI_Row == 0xE3) {
				AI_Row = 0xF1;
				upperAIPaddleRow = 0xFD;
				middleAIPaddleRow = 0xFB;
				lowerAIPaddleRow = 0XF7;
			}
			break;
		case AI_shiftDown:
			if (AI_Row == 0xF1) {
				AI_Row = 0xE3;
				upperAIPaddleRow = 0xFB;
				middleAIPaddleRow = 0xF7;
				lowerAIPaddleRow = 0XEF;
			}
			else if (AI_Row == 0xF8) {
				AI_Row = 0xF1;
				upperAIPaddleRow = 0xFD;
				middleAIPaddleRow = 0xFB;
				lowerAIPaddleRow = 0XF7;
			}
			break;
		case AI_release:
			// inizlizes random number gnerator
			srand(time(NULL));
			AI_Random = rand() % 2;
			break;
	}
	//PORTC = AI_Pattern; // Pattern to display (play2 and AI all stay the same pattern)
	//PORTD = AI_Row;	 // Row(s) displaying pattern
	return state;
}

// task for ball movement
// for ball  tatsk
unsigned char isPlayer2 = 0x00; // for check if plyaer 2 or not
unsigned char isPlay1LostRound = 0x00; // for check if play 1 lost round or not, 
unsigned char isPlay2LostRound = 0x00; // otherwise is player 2 or AI lost round
unsigned char isCorner = 0x00; // for check the ball hitting the corner
// move left (1), move right(2), move left up(3), move left down(4), move right up(5), move right down(6), player1 lose round(7), player2/AI lose round(8)
enum Ball_States {B_Start, B_init,B_move, B_moveL,B_moveR, B_leftUp,B_leftDown, B_rightUp, B_rightDown, B_play1Lose, B_play2Lose}; 
int Ball_Tick(int state) {
	
	// for check the game start or not
	// for checking the paddle position of the second player or AI
	static unsigned char uPaddleRow; 
	static unsigned char mPaddleRow;
	static unsigned char lPaddleRow;
	if (isBeginAI) { // if is AI mode we against AI paddle
		uPaddleRow = upperAIPaddleRow; 
		mPaddleRow = middleAIPaddleRow;
		lPaddleRow = lowerAIPaddleRow;
	}
	else { // if is player 2 mode we agins player 2 paddle
		uPaddleRow = upperPaddleRow2; 
		mPaddleRow = middlePaddleRow2;
		lPaddleRow = lowerPaddleRow2;
	}
	// Transitions
	switch (state) {
		case B_Start:
			if (isBeginAI || isBeginP2) {
				state = B_init;			
			}
			else {
				state = B_Start;			
			}
			break;
		case B_init:
			state = B_move;
			break;
		case B_move:
			// check player 1 lose or not
			if ( (Ball_Row != upperPaddleRow) && (Ball_Row != middlePaddleRow) && (Ball_Row != lowerPaddleRow) && (Ball_Pattern == 0x01) ) {
				moveType = 0x07; // it's represent player 1 lose round
				isReset	= 0x01;		
			}
			else if ((Ball_Row != uPaddleRow) && (Ball_Row != mPaddleRow) && (Ball_Row != lPaddleRow) && (Ball_Pattern == 0x80)) {
				moveType = 0x08; // it's represent play 2 lose round
				isReset = 0x01; 
			}
			// AI/player 2 paddle hit (left side)
			// left down hit upper and lower paddle
			else if ( (Ball_Row == uPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x04) ) { // left down and hit the upper paddle
				moveType = 0x05; // direction to right up(bouncing back)
			}
			else if ((Ball_Row == lPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x04) && (Ball_Row == 0xEF)) { // left down and hit the lower paddle at row 5 (case 1)
				moveType = 0x05; // direction to right up(bouncing back)			
					
			}
			else if ((lPaddleRow == 0xF7) && (Ball_Pattern == 0x40) && (moveType == 0x04) && (Ball_Row == 0xEF)) { // left down and hit the lower paddle at row 5 (case 2)
				moveType = 0x05; // direction to right up(bouncing back)			
					
			}
			else if ((Ball_Row == ((uPaddleRow>>1)  | 0x80)) && (Ball_Pattern == 0x40) && (moveType == 0x04) && (Ball_Row == 0xFD)) { // left down and hit the upper paddle at row 2
				moveType = 0x05; // direction to right up(bouncing back)			
					
			}
			else if ((Ball_Row == lPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x04)) { // left down and hit the lower paddle
				moveType = 0x06; // direction to right down			
					
			}
			// left up hit upper and lower paddle
			else if ( (Ball_Row == uPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x03) && (Ball_Row == 0xFE)) { // left up and hit the upper paddle at row 1 (case 1)
				moveType = 0x06; // direction to right down (bouncing back)
			}
			else if ( (Ball_Row == ((uPaddleRow >>1) | 0x80)) && (Ball_Pattern == 0x40) && (moveType == 0x03) && (Ball_Row == 0xFE)) { // left up and hit the upper paddle at row 1 (case 2)
				moveType = 0x06; // direction to right down (bouncing back)
			}
			else if ( (lPaddleRow == 0xFB) && (Ball_Pattern == 0x40) && (moveType == 0x03) && (Ball_Row == 0xF7)) { // left up and hit the LOWER paddle at row 4
				moveType = 0x06; // direction to right down (bouncing back)
			}
			else if ( (Ball_Row == uPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x03) ) { // left up and hit the upper paddle
				moveType = 0x05; // direction to right up
			}
			else if ((Ball_Row == lPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x03)) { // left up and hit the lower paddle
				moveType = 0x06; // direction to right down(bouncing back)			
					
			}
			// left up and down hit middle paddle
			else if ((Ball_Row == mPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x04)) { // left down and hit the middle paddle
				moveType = 0x02; // to right
			}
			else if ((Ball_Row == mPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x03)) { // left up and hit the middle paddle
				moveType = 0x02; // to right
			}

			// ball hit player 1 paddle
			// right down hit upper and lower paddle
			else if ( (Ball_Row == upperPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x06) ) { // right down and hit the upper paddle
				moveType = 0x03; // direction to left up(bouncing back)
			}
			else if ((Ball_Row == lowerPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x06) && (Ball_Row == 0xEF)) { // right down and hit the lower paddle at row 5 (case 1)
				moveType = 0x03; // direction to left UP (bouncing back)			
					
			}
			else if ((lowerPaddleRow == 0xF7) && (Ball_Pattern == 0x02) && (moveType == 0x06) && (Ball_Row == 0xEF)) { // right down and hit the lower paddle at row 5 (case 2)
				moveType = 0x03; // direction to left UP (bouncing back)			
					
			}
			else if ((Ball_Row == ((upperPaddleRow>>1)|0x80)) && (Ball_Pattern == 0x02) && (moveType == 0x06) && (Ball_Row == 0xFD)) { // right down and hit the upper paddle at row 2
				moveType = 0x03; // direction to left UP (bouncing back)			
					
			}
			else if ((Ball_Row == lowerPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x06)) { // right down and hit the lower paddle
				moveType = 0x04; // direction to left down			
					
			}
			// right up hit upper and lower paddle
			else if ( (Ball_Row == upperPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x05) && (Ball_Row == 0xFE)) { // right up and hit the upper paddle at row 1 (case 1)
				moveType = 0x04; // direction to left downdown(bouncing back)
			}
			else if ( (Ball_Row == ((upperPaddleRow>>1)|0x80)) && (Ball_Pattern == 0x02) && (moveType == 0x05) && (Ball_Row == 0xFE)) { // right up and hit the upper paddle at row 1 (case 2)
				moveType = 0x04; // direction to left downdown(bouncing back)
			}
			else if ((lowerPaddleRow == 0xFB) && (Ball_Pattern == 0x02) && (moveType == 0x05) && (Ball_Row == 0xF7)) { // right up and hit the lower paddle at row 4
				moveType = 0x04; // direction to left downdown(bouncing back)
			}
			else if ( (Ball_Row == upperPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x05) ) { // right up and hit the upper paddle
				moveType = 0x03; // direction to left up
			}
			else if ((Ball_Row == lowerPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x05)) { // right up and hit the lower paddle
				moveType = 0x04; // direction to left down(bouncing back)				
			}
			// right up and down hit middle paddle
			else if ((Ball_Row == middlePaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x06)) { // rigtht down and hit the middle paddle
				moveType = 0x01;
			}
			else if ((Ball_Row == middlePaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x05)) { // right up and hit the middle paddle
				moveType = 0x01;
			}

			// striaght bounce for player 1 paddle
			else if ((Ball_Row == middlePaddleRow || isSpin == 0x01 || isSpin == 0x02) && (Ball_Pattern == 0x02) && (moveType == 0x02)) { // if right and hit middle of paddle
				if (isSpin == 0x01) {
					moveType =0x04;				
				}
				else if (isSpin == 0x02) {
					moveType = 0x03;
				}
				else {
					moveType = 0x01; // direction to left
				}
						
			}
			else if ((Ball_Row == upperPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x02)) { // if right and hit upper of paddle 
				moveType = 0x03; // direction to left up		
			}
			else if ((Ball_Row == lowerPaddleRow) && (Ball_Pattern == 0x02) && (moveType == 0x02)) { // if right and hit lower of paddle
				moveType = 0x04; // direction to left down		
			}
	
			// striaght bounce for AI/player 2 paddle
			else if ((Ball_Row == mPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x01)) { // if left end hit middle paddle
				moveType = 0x02; // direction to right
			}
			else if ((Ball_Row == uPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x01)) { // if left and hit upper of paddle 
				moveType = 0x05; // direction to right up		
			}
			else if ((Ball_Row == lPaddleRow) && (Ball_Pattern == 0x40) && (moveType == 0x01)) { // if left and hit lower of paddle
				moveType = 0x06; // direction to right down		
			}


			// 4 corner bounce
			// corner case, lose case 
			else if ((moveType == 0x03) && (Ball_Row == 0xFE) && (Ball_Pattern == 0x80)) { // upper left corner
				moveType = 0x06; // dirction to right down		
			}
			else if ((moveType == 0x04) && (Ball_Row == 0xEF) && (Ball_Pattern == 0x80)) { // lower left corner
				moveType = 0x05; // direction to right up
			}
			else if ((moveType == 0x05) && (Ball_Row == 0xFE) && (Ball_Pattern == 0x01)) { // upper right corner
				moveType = 0x04; // direction to left down
			}
			else if ((moveType == 0x06) && (Ball_Row == 0xEF) && (Ball_Pattern == 0x01)) { // lower right corner
				moveType = 0x03; // dirction to left up
			}
			
			
			
			// below are consider ball bouncing by hiting walls (4 walls: upper, lower, right, left)
			// upper wall, and lower wall
			// left direction bounce
			else if ( (moveType == 0x03) && (Ball_Row == 0xFE)) { // left up and hit wall
				moveType = 0x04; // direction to left down
			}
			else if ( (moveType == 0x04) && (Ball_Row == 0xEF) ) { // left down and hit wall
				moveType = 0x03; // directon to left up			 
			}
			// right direction bounce
			else if ( (moveType == 0x05) && (Ball_Row == 0xFE) ) { // right up and hit wall
				moveType = 0x06; // direction to right down
			}
			else if ( (moveType == 0x06) && (Ball_Row == 0xEF) ) { // right down and hit wall
				moveType = 0x05; // directon to right up			
			}
			
			// left wall and right wall
			// left wall bounce
			else if ( (moveType == 0x03) && (Ball_Pattern == 0x80) ) { // left up and hit right wall
				moveType = 0x05; // direction to right up			
			}
			else if ( (moveType == 0x04) && (Ball_Pattern == 0x80) ) { // left down and hit right wall
				moveType = 0x06; // direction to right down			
			}
			// right wall bounce
			else if ( (moveType == 0x05) && (Ball_Pattern == 0x01) ) { // right up and hit right wall
				moveType = 0x03; // direction to left up			
			}
			else if ( (moveType == 0x06) && (Ball_Pattern == 0x01) ) { // right down and hit right wall
				moveType = 0x04; // direction to left down			
			}
			
			// ball speed check, if we need to check the ball speed or not
			if ((moveType == 0x01) || (moveType == 0x02)) {
				isCorner = 0x00;	
			}
			else {
				isCorner = 0x01;
				isSpin =0x00;			
			}


			// direction state check
			if ((moveType) == 0x01) {
				state = B_moveL;
			}
			else if ((moveType) == 0x02) {
				state = B_moveR;
			}
			else if ((moveType) == 0x03) {
				state = B_leftUp;
			}
			else if ((moveType) == 0x04) {
				state = B_leftDown;
			}
			else if ((moveType) == 0x05) {
				state = B_rightUp;
			}
			else if ((moveType) == 0x06) {
				state = B_rightDown;
			}
			else if ((moveType) == 0x07) {
				state = B_play1Lose;
			}
			else if ((moveType) == 0x08) {
				state = B_play2Lose;
			}
			else{
				state = B_move;
			}
			break;
		case B_moveL:
			state = B_move;
			break;
		case B_moveR:
			state = B_move;
			break;
		case B_leftDown:
			state = B_move;
			break;
		case B_leftUp:
			state = B_move;
			break;
		case B_rightDown:
			state = B_move;
			break;
		case B_rightUp:
			state = B_move;
			break;
		case B_play1Lose:
			state = B_init;
			break;
		case B_play2Lose:
			state = B_init;
			break;
		default:
			state = B_Start;
			break;
	}
	// Actions
	switch (state) {
		case B_Start:
			break;
		case B_init:
			if (isPlay1LostRound) {
				// reset the ball
				Ball_Pattern = 0x10;	
				Ball_Row = 0xFB;  
				moveType = 0x02;
			}
			else {
				// reset the ball
				Ball_Pattern = 0x08;	
				Ball_Row = 0xFB;  
				moveType = 0x01;
			}
			break;
		case B_move:
			break;
		case B_moveL:
			Ball_Pattern <<= 1; // Shift LED one spot to the left on current row
			break;
		case B_moveR:
			Ball_Pattern >>= 1; // Shift LED one spot to the right on current row
			break;
		case B_leftDown:
			Ball_Row = (Ball_Row << 1) | 0x01; // row down one
			Ball_Pattern <<= 1; // Shift LED one spot to the left on current row
			break;
		case B_leftUp:
			Ball_Row = (Ball_Row >> 1) | 0x80; // row up one
			Ball_Pattern <<= 1; // Shift LED one spot to the left on current row
			break;
		case B_rightDown:
			Ball_Row = (Ball_Row << 1) | 0x01; // row down one
			Ball_Pattern >>= 1; // Shift LED one spot to the right on current row
			break;
		case B_rightUp:
			Ball_Row = (Ball_Row >> 1) | 0x80; // row up one
			Ball_Pattern >>= 1; // Shift LED one spot to the left on current row
			break;
		case B_play1Lose:
			isPlay1LostRound = 0x01;
			score2++; // player 2 get 1 point
			break;
		case B_play2Lose:
			isPlay1LostRound = 0x00;
			score1++; // player 1 get 1 point
			break;
	}
	//PORTC = Ball_Pattern;	// Pattern to display
	//PORTD = Ball_Row;		// Row(s) displaying pattern
	if (score2 == 0x01) {
		temp2 = 0x40;
	}
	else if (score2 == 0x02){
		temp2 = 0x60;
	}
	else  if (score2 == 0x03) {
		temp2 = 0x70;
	}
	if (score1 == 0x01) {
		temp1 = 0x80;
	}
	else if (score1 == 0x02){
		temp1 = 0xC0;
	}
	else  if (score1 == 0x03) {
		temp1 = 0xE0;
	}
	return state;
}

enum P_States {shiftR, shiftL};
unsigned char P_pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
unsigned char P_row = 0x00;  	// Row(s) displaying pattern.
int Pause_Tick(int state) {
	// Transitions
	switch (state) {
		case shiftR:
			if (P_pattern == 0x01) {
				state = shiftL;			
			}
			else {
				state = shiftR;			
			}
			break;
		case shiftL:
			if (P_pattern == 0x80) {
				state = shiftR;			
			}
			else {
				state = shiftL;			
			}	
			break;
		default:	
			state = shiftR;
			break;
	}	
	// Actions
	switch (state) {
		case shiftR:
			// Shift LED one spot to the right on current row
			P_pattern >>= 1;
			break;
		case shiftL:
			// Shift LED one spot to the right on current row
			P_pattern <<= 1;
			break;
		default:
			break;
	}
	//PORTC = P_pattern;	// Pattern to display
	//PORTD = P_row;		// Row(s) displaying pattern	
	return state;
}
// output state machine
enum O_States {O_Start,O_begin, O_init,O_play1, O_play2, O_Ball, O_PauseDisplay, O_reset, O_resetR};
int Output_Tick (int state) {
	unsigned char PB_2;
	unsigned char PB_3;
	PB_2 = ~PINB & 0x04; // for button PB2
	PB_3 = ~PINB & 0x08; // For button PB3
	switch (state) {
		case O_Start:
			if (PB_2 && !PB_3) {
				state = O_begin;
				isBeginAI = 0x01;
				isBeginP2 = 0x00;
			}
			else if (PB_3 && !PB_2) {
				state = O_begin;
				isBeginP2 = 0x01;
				isBeginAI = 0x00;
			}
			else {
				state = O_Start;
			}
			break;
		case O_begin:
			state = O_init;
			break;
		case O_init:
			if (PB_2 && PB_3) {
				state = O_reset;
			}
			else if (score1 == 0x03 || score2 == 0x03) {
				state = O_Start;
				isBeginAI = 0x00;
				isBeginP2 = 0x00;
				score1 = 0x00;
				score2 = 0x00;
			}
			else {
				state = O_play1;
			}
						
			break;
		case O_play1:
			state = O_play2;
			break;
		case O_play2:
			state = O_Ball;
			break;
		case O_Ball:
			state = O_init;
			break;
		case O_reset: 
			state = O_resetR;
			break;
		case O_resetR:
			if (!PB_3 && !PB_2) {
				state = O_Start;
			}
			else {
				state = O_resetR;
			}
			break;
		default:
			
			state = O_Start;
			break;	
	}
	switch (state) {
		case O_Start:
			//PORTA = PORTA & 0xE0;
			//PORTB = PORTB & 0x70;
			PORTC = P_pattern;
			PORTD = P_row;
			break;
		case O_begin:
			//PORTA = PORTB & 0x03;
			PORTB = PORTB & 0x0F;
			break;
		case O_init:
			break;
		case O_play1:
			PORTA = PORTB | temp1;
			PORTB = PORTB | temp2;
			PORTC = Player1_Pattern;	// Pattern to display
			PORTD = Player1_Row;
			break;
		case O_play2:
			if (isBeginAI) {
				PORTC = AI_Pattern; // Pattern to display (play2 and AI all stay the same pattern)
				PORTD = AI_Row;	 // Row(s) displaying pattern		// Row(s) displaying pattern
			}
			else if (isBeginP2) {
				PORTC = Player2_Pattern; // Pattern to display (play2 and AI all stay the same pattern)
				PORTD = Player2_Row;	 // Row(s) displaying pattern		// Row(s) displaying pattern
			}
			PORTA = PORTA | temp1;
			PORTB = PORTB | temp2;
			break;
		case O_Ball:
			PORTA = PORTA | temp1;
			PORTB = PORTB | temp2;
			PORTC = Ball_Pattern;	// Pattern to display
			PORTD = Ball_Row;		// Row(s) displaying pattern
			break;
		case O_reset: 
			isBeginAI = 0x00;
			isBeginP2 = 0x00;
			score1 = 0x00;
			score2 = 0x00;
			break;
		case O_resetR:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0xFC; PORTA = 0x03; //
	DDRB = 0xF0; PORTB = 0x0F; //
	DDRC = 0xFF; PORTC = 0x00; //
	DDRD = 0x1F; PORTD = 0xE0; //
    /* Insert your solution below */
	// Declare an array of tasks
	static task taskP,task1,task2,task3,task4,taskOuput;
	task *tasks[] = {&taskP,&task1,&task2,&task3,&task4, &taskOuput};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;
	// Task 1 (Play1_Tick)
	task1.state = start; // task initial state.
	task1.period = 50; //Task period.
	task1.elapsedTime = task1.period; // Task current elasped time.
	task1.TickFct = &Play1_Tick; // Function pointer for the tick.

	// Task 2 (AI_Tick)
	task2.state = start; // task initial state.
	task2.period = 10; //Task period.
	task2.elapsedTime = task2.period; // Task current elasped time.
	task2.TickFct = &AI_Tick; // Function pointer for the tick.

	// Task 3 (Ball_Tick)
	task3.state = start; // task initial state.
	task3.period = 200; //Task period.
	task3.elapsedTime = task3.period; // Task current elasped time.
	task3.TickFct = &Ball_Tick; // Function pointer for the tick.

	// Task 4 (Play2_Tick)
	task4.state = start; // task initial state.
	task4.period = 100; //Task period.
	task4.elapsedTime = task4.period; // Task current elasped time.
	task4.TickFct = &Play2_Tick; // Function pointer for the tick.

	// Task  (Pause_Tick)
	taskP.state = start; // task initial state.
	taskP.period = 100; //Task period.
	taskP.elapsedTime = taskP.period; // Task current elasped time.
	taskP.TickFct = &Pause_Tick; // Function pointer for the tick.

	// Task for output (Output_Tick)
	taskOuput.state = start; // task initial state.
	taskOuput.period = 1; //Task period.
	taskOuput.elapsedTime = taskOuput.period; // Task current elasped time.
	taskOuput.TickFct = &Output_Tick; // Function pointer for the tick.

	unsigned short i; // GCD and Scheduler for-loop iterator
	// find the GCD
	unsigned long GCD = tasks[0]->period;
	for (i = 1; i<numTasks; i++) {
		GCD = findGCD(GCD, tasks[i]->period);
	}
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

    while (1) {
	for (i = 0; i < numTasks; i++) { // Scheduler code
		if (tasks[i]->elapsedTime == tasks[i]->period) { // Tasak is ready to tick
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state); // set next state
			tasks[i]->elapsedTime = 0; // Reset the elapsed time for next tick.
			if (isCorner) {
				task3.period = 120;
			}
			else if ((isSpin == 0x01) || isSpin == (0x02)) {
				task3.period = 80;			
			}
			else {
				task3.period = 200;			
			}
			
		}
		tasks[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
