//OneFuncButton library v1.0, Copyright (c) 2017 Andrija Antunoviæ
//https://github.com/athnix/OneFuncButton/

#ifndef _OneFuncButton_h
#define _OneFuncButton_h

#include "Arduino.h"

#define DEBOUNCING_TIME 15

#define UP                 0b000000
#define DOWN_SHORT         0b000001
#define DOWN_LONG_FIRST    0b000011
#define DOWN_LONG_SUBSQ    0b000111
#define PRESS_SHORT        0b001001
#define PRESS_LONG_FIRST   0b001011
#define PRESS_LONG_SUBSQ   0b001111
#define RELEASE_SHORT      0b010000
#define RELEASE_LONG_FIRST 0b010010
#define RELEASE_LONG_SUBSQ 0b010110
#define DEBOUNCING_UP      0b100000
#define DEBOUNCING_DOWN    0b100001
//                           | ||||  BIT MEANINGS:
//                           | |||`- Button is down?
//                           | ||`-- Short press (0) or long press (1)?
//                           | |`--- If long press phase, first longpress phase (0) or subsequent longpress phase (1)?
//                           | `---- Type of event? 00 - no event (static state), 01 - press, 10 - release
//                           `------ Debouncing? (only internal states utilize this bit)

#define ISDOWN(X)          (X & 1)         //Evaluates true while the button is down
#define PRESSEVENT(X)      (X & 0b001000)  //Evaluates true when any press event occurs
#define RELEASEEVENT(X)    (X & 0b010000)  //Evaluates true when any release event occurs

class Button {
private:
	byte pin;
	byte activeState;
	unsigned long timeForNextChange;	//Time variable
	byte btnState;						//State variable
	
public:
	unsigned int firstHoldTime; //Time it takes for a long-press event (PRESS_LONG_FIRST) to be triggered for the first time after pushing down the button
	unsigned int subsqHoldTime; //Time it takes for a long-press event (PRESS_LONG_SUBSQ) to be triggered after it was already triggered in this down state

	Button(int, bool = HIGH, unsigned int = 800, unsigned int = 200); //Class constructor
	byte state(); //The function, evaluates and returns the button state
};

#endif
