#include "OneFuncButtonLib.h"

Button btn1(3); //Creating a button object (only the pin argument is mandatory)

//We can also provide custom firstHoldTime and subsqHoldTime durations:
//Button btn1(3, HIGH, 500, 300);

//...or make the button active-low:
//Button btn1(3, LOW);

void setup() {
	Serial.begin(9600);
}

void loop() {
	//Example program which prints out button events in Serial.

	byte s = btn1.state(); //state() function both evaluates and returns the state of the button as a single byte value

	switch (s)
	{
	case PRESS_SHORT: Serial.println("Short-press"); break;
	case PRESS_LONG_FIRST: Serial.println("First long-press trigger"); break;
	case PRESS_LONG_SUBSQ: Serial.println("Subsequent long-press trigger"); break;
	case RELEASE_SHORT: Serial.println("Release after short-press"); break;
	case RELEASE_LONG_FIRST: Serial.println("Release after first long-press"); break;
	case RELEASE_LONG_SUBSQ: Serial.println("Release after subsequent long-press"); break;
	}
}