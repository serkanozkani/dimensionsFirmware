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

PoluluStepper::PoluluStepper()
{
	_stepPin = -1;
	_enablePin = -1;
	_directionPin = -1;
	
	_enabled = false;
}

PoluluStepper::PoluluStepper(int stepPin, int enablePin, int directionPin)
{
	_stepPin = stepPin;
	_enablePin = enablePin;
	_directionPin = directionPin;

	_enabled = false;

	disable();
}

void PoluluStepper::setup()
{
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

void PoluluStepper::setDirection (bool clockwise)
{
	if (clockwise) {
		digitalWrite(_directionPin, LOW);
	} else {
		digitalWrite(_directionPin, HIGH);
	}
}

void PoluluStepper::revolve (bool clockwise)
{
	enable();

	setDirection(clockwise);

	for (unsigned int i = MOTOR_STEPS_PER_REVOLUTION; i--; ) {
		digitalWrite(_stepPin, HIGH);
		delayMicroseconds(DELAY_US_PER_HALF_STEP_NORMAL);
		digitalWrite(_stepPin, LOW);
		delayMicroseconds(DELAY_US_PER_HALF_STEP_NORMAL);
	}
}

void PoluluStepper::rotate( unsigned int numSteps, bool clockwise)
{
	enable();

	setDirection(clockwise);

	for (unsigned int i = numSteps; i--; ) {
		digitalWrite(_stepPin, HIGH);
		delayMicroseconds(DELAY_US_PER_HALF_STEP_NORMAL);
		digitalWrite(_stepPin, LOW);
		delayMicroseconds(DELAY_US_PER_HALF_STEP_NORMAL);
	}
}
