/*	* LedIndicator.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#include "LedIndicator.h"
#include "Arduino.h"

LedIndicator::LedIndicator()
{
	_pin = -1;
	_on = false;
}

LedIndicator::LedIndicator(int interfacePin)
{
	_pin = interfacePin;
	_on = false;
}

void LedIndicator::setup()
{
	pinMode(_pin, OUTPUT);
	off();
}


void LedIndicator::on()
{
	if (_on == false) {
		_on = true;
		digitalWrite(_pin, HIGH);
	}
}

void LedIndicator::off()
{
	if (_on == true) {
		_on = false;
		digitalWrite(_pin, LOW);
	}
}