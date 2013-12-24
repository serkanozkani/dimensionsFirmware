/*	* LinearMotionController.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-23
	*/


#include "LinearMotionController.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Arduino.h"

LinearMotionController::LinearMotionController()
{
}

void LinearMotionController::setup(PoluluStepper *motor, EndstopSwitch *endstopSwitch)
{
	_motor = motor;
	_endstop = endstopSwitch;
}


void LinearMotionController::calibrate ()
{
	_movingToTarget = false;
	_zeroing = true;
	_actionCycles = 0;
	_motor->enable();
	_motor->slow();
}

unsigned int LinearMotionController::moveTo(unsigned int stepsFromOrigin)
{
	if (!_zeroed) {
		return 1;
	}

	_startingDirection = (_stepsFromOrigin > stepsFromOrigin);

	if (_startingDirection) {
		_stepsFromTarget = _stepsFromOrigin - stepsFromOrigin;
	} else {
		_stepsFromTarget = stepsFromOrigin - _stepsFromOrigin;
	}
	
	_motor->setDirection(_startingDirection);
	_motor->normal();

	_movingToTarget = true;
	_actionCycles = 0;
	_zeroing = false;
	
}

void LinearMotionController::singleRevolution()
{
	unsigned int _actionCycles = 0;

	while(true) {
		if (_actionCycles == 0) {
			_motor->enable();
			_motor->slow();
		} else if (_actionCycles == 1000) {
			_motor->revolve(false);
		} else if (_actionCycles == 2000) {
			_motor->revolve(true);
		} else if (_actionCycles == 3000) {
			return;
		}
		_actionCycles++;
	}
}

void LinearMotionController::loop(unsigned long now) {

	if (_stepsFromOrigin < 0) {
		Serial.println(_stepsFromOrigin);
		return;
	}

	if (_zeroing) {
		if (_endstop->triggered()) {

			if (_zeroing) {
				_motor->slow();
				while (_endstop->triggered()) {
					_motor->rotate(1, false);
				}
				_done();
			}
			_actionCycles = 0;
			return;
		}

		if (_actionCycles == 10) {
			_motor->normal();
		} else if (_actionCycles == 100) {
			_motor->fast();
		}

		_motor->rotate(10, true);
	}

	if (_movingToTarget) {

		int stepsToMove = 0;

		if (_stepsFromTarget == 0) {
			_done();
			return;
		} else if (_stepsFromTarget > 10) {

			if (_stepsFromTarget <= 1000 && _actionCycles >= 10) {
				_motor->normal();
			} else if (_actionCycles >= 100) {
				_motor->fast();
			}

			stepsToMove = 10;

		} else if (_stepsFromTarget > 0) {
			stepsToMove = _stepsFromTarget;

			_motor->slow();
		}

		if (stepsToMove) {
			_motor->rotate(stepsToMove);
			_stepsFromTarget -= stepsToMove;
			if (_startingDirection) {
				_stepsFromOrigin -= stepsToMove;
			} else {
				_stepsFromOrigin += stepsToMove;
			}
		}
	}

	_actionCycles++;

	return;
}

unsigned int LinearMotionController::status() {
	unsigned int statusCode = 0;
	if (_zeroing) {
		statusCode |= 0x1;
	}
	if (_zeroed) {
		statusCode |= 0x2;
	}
	if (_movingToTarget) {
		statusCode |= 0x4;
	}
	if (_actionCycles != 0) {
		statusCode |= 0x8;
	}
	return statusCode;
}

bool LinearMotionController::ready() {
	if (_actionCycles != 0) return false;
	if (_zeroing || _movingToTarget) return false;
	
	return true;
}

void LinearMotionController::reset()
{
	_actionCycles = 0;
	_movingToTarget = false;
	_zeroing = false;
	_zeroed = false;
}

void LinearMotionController::_done () 
{
	_stepsFromTarget = 0;
	_movingToTarget = false;
	_zeroing = false;
	_zeroed = true;
}