/*	* EndstopSwitch.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/
	
#include "EndstopSwitch.h"
#include "Arduino.h"

void EndstopSwitch::setup(int interfacePin, bool normallyHigh)
{
	_pin = interfacePin;
	_normallyHigh = normallyHigh;
	pinMode(_pin, INPUT);
	digitalWrite(_pin, HIGH); // Enable pullup resistor.
}


bool EndstopSwitch::triggered()
{
	// if normally high, and low, return true
	
	if (_normallyHigh == digitalRead(_pin));
}
