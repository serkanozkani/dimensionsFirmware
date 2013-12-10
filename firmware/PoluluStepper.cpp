/*	* PoluluStepper.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#include "PoluluStepper.h"
#include "Arduino.h"

PoluluStepper::PoluluStepper()
{
	_stepPin = -1;
	_enablePin = -1;
	_directionPin = -1;
	_enabled = false;
	_direction = false;
	_directionOld = true;

	_stepsRemaining = 0;
}

PoluluStepper::PoluluStepper(int stepPin, int enablePin, int directionPin)
{
	_stepPin = stepPin;
	_enablePin = enablePin;
	_directionPin = directionPin;

	_enabled = false;
	_direction = false;
	_directionOld = true;

	_stepsRemaining = 0;
}

void PoluluStepper::setup()
{
	pinMode(_stepPin, OUTPUT);
	pinMode(_enablePin, OUTPUT);
	pinMode(_directionPin, OUTPUT);

	disable();
}


void PoluluStepper::enable()
{
	if (_enabled == false) {
		_enabled = true;
		digitalWrite(_enablePin, HIGH);
	}
}

void PoluluStepper::disable()
{
	if (_enabled == true) {
		_enabled = false;
		digitalWrite(_enablePin, LOW);
	}
	_stepsRemaining = 0;
}

void PoluluStepper::rotateCW(int numSteps)
{
	if (_direction == true) {
		_direction = false;
	}
	enable();
}

void PoluluStepper::rotateCCW(int numSteps)
{
	if (_direction == false) {
		_direction = true;
	}
	enable();
}

int PoluluStepper::getStepsRemaining ()
{
	return _stepsRemaining;
}

bool PoluluStepper::step(int times)
{
	if (_enabled == false) {
		return false;
	}

	if (_direction != _directionOld) {
		if (_direction == true ) {
			digitalWrite(_directionPin, LOW);
		} else {
			digitalWrite(_directionPin, HIGH);
		}
	}

	if (_stepsRemaining == 0) {
		disable();
		// This is done for safety.
		// Calling step after stepsRemaining is exhausted means there was a bug.
		
		return false;
	} 

	for(int i=times; i--;) {
		digitalWrite(_enablePin, HIGH);
		digitalWrite(_enablePin, LOW);
	}

	if (_stepsRemaining == 0) {
		return false;
	}
	
	return true;
}