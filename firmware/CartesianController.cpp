/*	* CartesianController.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-16
	*/


#include "CartesianController.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"

CartesianController::CartesianController()
{
	_motorX = (Ramps::instance()).getMotorX();
	_endstopX = (Ramps::instance()).getEndstopX();
	_motorY = (Ramps::instance()).getMotorY();
	_endstopY = (Ramps::instance()).getEndstopY();
}

void CartesianController::zeroX () 
{
	_zeroedX = true;
	_zeroingX = false;
	_sfoX = 0;
}

void CartesianController::zeroY () 
{
	_zeroedY = true;
	_zeroingY = false;
	_sfoY = 0;
}

void CartesianController::calibrateX ()
{
	_zeroingX = true;
}

void CartesianController::calibrateY ()
{
	_zeroingY = true;
}

void CartesianController::loop(unsigned long now) {

	if (_actionCycles == 0) {
		if (_zeroingX) {
			_motorX->enable();
			_motorX->slow();
		} else if (_zeroingY) {
			_motorY->enable();
			_motorY->slow();
		}
	}

	if ((_zeroingX && _endstopX->triggered()) || 
		(_zeroingY && _endstopY->triggered())) {

		if (_zeroingX) {
			_motorX->slow();
			while (_endstopX->triggered()) {
				_motorX->rotate(1, false);
			}
			zeroX();
		} else if (_zeroingY) {
			_motorY->slow();
			while (_endstopY->triggered()) {
				_motorY->rotate(1, false);
			}
			zeroY();
		}
		_actionCycles = 0;
		return;
	}

	if (_zeroingY) {
		if (_actionCycles == 10) {
			_motorY->normal();
		} else if (_actionCycles == 100) {
			_motorY->fast();
		} else if (_actionCycles == 1200) {
			_motorY->normal();
		} else if (_actionCycles == 1300) {
			_motorY->slow();
		}
		_motorY->rotate(10, true);
	} else if (_zeroingX) {
		if (_actionCycles == 10) {
			_motorX->normal();
		} else if (_actionCycles == 100) {
			_motorX->fast();
		} else if (_actionCycles == 1200) {
			_motorX->normal();
		} else if (_actionCycles == 1300) {
			_motorX->slow();
		}
		_motorX->rotate(10, true);
	}

	_actionCycles++;

	return;
}

bool CartesianController::ready() {
	if (_actionCycles != 0) return false;
	if (_zeroingX || _zeroingY) return false;
	
	return true;
}

void CartesianController::reset()
{
	_actionCycles = 0;
	_zeroingX = false;
	_zeroingY = false;
	_zeroedX = false;
	_zeroedY = false;
}





