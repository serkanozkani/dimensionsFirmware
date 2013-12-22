/*	* ExtruderController.cpp
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
#include "Thermistor.h"
#include "Arduino.h"


#include "ExtruderController.h"
#include "HeaterController.h"

void ExtruderController::setup(PoluluStepper &extruderMotor, HeaterController &heaterController)
{
	_extruderMotor = &extruderMotor;
	_heaterController = &heaterController;

	// No need to setup the extruder motor. Should be done via ramps.cpp
	setRate(0);
	setTemp(0);
	_errorCode = 0;
	
}

void ExtruderController::disable(int errorCode)
{
	_heaterController->disable(errorCode);
	_extruderMotor->disable();

	_errorCode = errorCode;
}

int ExtruderController::enable()
{
	if (_errorCode != 0) {
		return _errorCode;
	}

	setRate(0);
	setTemp(0);
	_extruderMotor->enable();

	_heaterController->disable(0);
	_heaterController->enable();

	return 0;
}

int ExtruderController::getTemp()
{
	return _heaterController->getTemp();
}

void ExtruderController::setRate(int mmHrRate)
{
	_mmHrRate = mmHrRate;
}

void ExtruderController::setTemp(int degreesCelsius)
{
	return _heaterController->setTemp(degreesCelsius);
}

void ExtruderController::loop(long now)
{
	_heaterController->loop(now);

	if (_nextStepperCycle < now && _heaterController->getTemp() >= 200) {
		_nextStepperCycle = now + 100;

		_extruderMotor->rotate(_mmHrRate);
	}
}
