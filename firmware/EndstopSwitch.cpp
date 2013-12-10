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

EndstopSwitch::EndstopSwitch()
{
	_pin = -1;
	_normallyHigh = false;
}

EndstopSwitch::EndstopSwitch(int interfacePin, bool normallyHigh)
{
	_pin = interfacePin;
	_normallyHigh = normallyHigh;
}

void EndstopSwitch::setup()
{
	pinMode(_pin, INPUT);
}


bool EndstopSwitch::getState()
{
	// if normally high, and low, return true
	
	return (_normallyHigh != digitalRead(_pin));
}
