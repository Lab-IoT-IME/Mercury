#include "Button.h"

Button::Button(uint8_t pin, uint16_t debounce_ms)
:  _pin(pin)
,  _delay(debounce_ms)
,  _state(LOW)
,  _ignore_until(0)
,  _has_changed(false)
{}

void Button::begin()
{
    pinMode(_pin, INPUT);
}

bool Button::read()
{
	if (_ignore_until <= millis() && digitalRead(_pin) != _state)
	{
		_ignore_until = millis() + _delay;
		_state = !_state;
		_has_changed = true;
	}
	
	return _state;
}

bool Button::has_changed()
{
	if (_has_changed)
	{
		_has_changed = false;
		return true;
	}
	return false;
}

bool Button::toggled()
{
	read();
	return has_changed();
}

bool Button::pressed()
{
	return (read() == PRESSED && has_changed());
}

bool Button::released()
{
	return (read() == RELEASED && has_changed());
}