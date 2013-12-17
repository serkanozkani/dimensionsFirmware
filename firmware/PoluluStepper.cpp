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
#include "motors.h"
#include "Arduino.h"

void PoluluStepper::setup(int stepPin, int enablePin, int directionPin, bool startDirection)
{
	_stepPin = stepPin;
	_enablePin = enablePin;
	_directionPin = directionPin;
	_startDirection = startDirection;

	_enabled = false;

	pinMode(_stepPin, OUTPUT);
	pinMode(_enablePin, OUTPUT);
	pinMode(_directionPin, OUTPUT);

	disable();
}

void PoluluStepper::fast(){
	_delayPerHalfStep = DELAY_US_PER_HALF_STEP_FAST;
}

void PoluluStepper::slow(){
	_delayPerHalfStep = DELAY_US_PER_HALF_STEP_SLOW;
}

void PoluluStepper::normal(){
	_delayPerHalfStep = DELAY_US_PER_HALF_STEP_NORMAL;
}

void PoluluStepper::enable()
{
	if (_enabled == false) {
		_enabled = true;
		digitalWrite(_enablePin, LOW);
	}
}

void PoluluStepper::disable()
{
	_enabled = false;

	digitalWrite(_enablePin, HIGH);
}

void PoluluStepper::setDirection (bool forward)
{
	if (forward ^ _startDirection) {
		digitalWrite(_directionPin, HIGH);
	} else {
		digitalWrite(_directionPin, LOW);
	}
}

void PoluluStepper::revolve (bool forward)
{
	enable();

	setDirection(forward);

	for (unsigned int i = MOTOR_STEPS_PER_REVOLUTION; i--; ) {
		digitalWrite(_stepPin, HIGH);
		delayMicroseconds(_delayPerHalfStep);
		digitalWrite(_stepPin, LOW);
		delayMicroseconds(_delayPerHalfStep);
	}
}

void PoluluStepper::rotate( unsigned int numSteps, bool forward)
{
	setDirection(forward);
	rotate(numSteps);
}

void PoluluStepper::rotate( unsigned int numSteps)
{
	enable();

	for (unsigned int i = numSteps; i--; ) {
		digitalWrite(_stepPin, HIGH);
		delayMicroseconds(_delayPerHalfStep);
		digitalWrite(_stepPin, LOW);
		delayMicroseconds(_delayPerHalfStep);
	}
}
