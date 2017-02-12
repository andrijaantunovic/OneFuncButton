#include "OneFuncButtonLib.h"

Button::Button(int _pin, bool _activeState = HIGH, unsigned int _fht = 800, unsigned int _sht = 200)
{
	pin = _pin;
	activeState = _activeState;
	firstHoldTime = _fht;
	subsqHoldTime = _sht;
	pinMode(_pin, INPUT);

	btnState = UP;
	timeForNextChange = millis();
}

byte Button::state()
{
	switch (btnState)
	{
	case UP:
		if (digitalRead(pin) == activeState)
		{
			/* TIME */   timeForNextChange = millis() + DEBOUNCING_TIME;
			/* STATE */  btnState = DEBOUNCING_DOWN;
			/* RETURN */ return PRESS_SHORT;
		}
		return UP;

	case DOWN_SHORT:
		if (digitalRead(pin) != activeState)
		{
			/* TIME */   timeForNextChange = millis() + DEBOUNCING_TIME;
			/* STATE */  btnState = DEBOUNCING_UP;
			/* RETURN */ return RELEASE_SHORT;
		}
		else if (millis() >= timeForNextChange)
		{
			/* TIME */   timeForNextChange = millis() + subsqHoldTime;
			/* STATE */  btnState = DOWN_LONG_FIRST;
			/* RETURN */ return PRESS_LONG_FIRST;
		}
		return DOWN_SHORT;

	case DOWN_LONG_FIRST:
		if (digitalRead(pin) != activeState)
		{
			/* TIME */   timeForNextChange = millis() + DEBOUNCING_TIME;
			/* STATE */  btnState = DEBOUNCING_UP;
			/* RETURN */ return RELEASE_LONG_FIRST;
		}
		else if (millis() >= timeForNextChange)
		{
			/* TIME */   timeForNextChange = millis() + subsqHoldTime;
			/* STATE */  btnState = DOWN_LONG_SUBSQ;
			/* RETURN */ return PRESS_LONG_SUBSQ;
		}
		return DOWN_LONG_FIRST;

	case DOWN_LONG_SUBSQ:
		if (digitalRead(pin) != activeState)
		{
			/* TIME */   timeForNextChange = millis() + DEBOUNCING_TIME;
			/* STATE */  btnState = DEBOUNCING_UP;
			/* RETURN */ return RELEASE_LONG_SUBSQ;
		}
		else if (millis() >= timeForNextChange)
		{
			/* TIME */   timeForNextChange = millis() + subsqHoldTime;
			/* STATE */  btnState = DOWN_LONG_SUBSQ;
			/* RETURN */ return PRESS_LONG_SUBSQ;
		}
		return DOWN_LONG_SUBSQ;

	case DEBOUNCING_UP:
		if (millis() >= timeForNextChange)
		{
			/* STATE */  btnState = UP;
		}
		return UP;

	case DEBOUNCING_DOWN:
		if (millis() >= timeForNextChange)
		{
			/* TIME */   timeForNextChange = millis() + firstHoldTime;
			/* STATE */  btnState = DOWN_SHORT;
		}
		return DOWN_SHORT;
	}
}
