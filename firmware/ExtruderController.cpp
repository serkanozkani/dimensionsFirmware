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

ExtruderController::ExtruderController ()
{
}

ExtruderController::ExtruderController (PoluluStepper extruderMotor, int interfacePin, Thermistor hotendThermistor)
{
	_extruderMotor = extruderMotor;
	_hotendThermistor = hotendThermistor;
	_pin = interfacePin;
}

void ExtruderController::setup()
{
	// No need to setup the extruder motor. Should be done via ramps.cpp
	setRate(0);
	setTemp(0);
	_errorCode = 0;

	pinMode(_pin, OUTPUT);
	
}

void ExtruderController::disable(int errorCode)
{
	// Stop heating no matter what.
	digitalWrite(_pin, LOW);

	_extruderMotor.disable();

	// Handy tip for recovering from an overtemp:
	// disable(0), enable, setTemp, wait, getTemp, setRate and we're back.
	_errorCode = errorCode;
}

int ExtruderController::enable()
{
	if (_errorCode != 0) {
		return _errorCode;
	}

	setRate(0);
	setTemp(0);
	_extruderMotor.enable();

	return 0;
}

int ExtruderController::getTemp()
{
	if (_errorCode != 0) {
		return 9999;
	}

	return _hotendThermistor.getDegreesCelsius();
}

void ExtruderController::setRate(int mmHrRate)
{
	_mmHrRate = mmHrRate;
}

void ExtruderController::setTemp(int degreesCelsius)
{
	_targetTemp = degreesCelsius;
}

void ExtruderController::loop(unsigned long now)
{
	int degreesCelsius = getTemp();

	if (degreesCelsius < _targetTemp && _targetTemp < 350) {
		
		// Cool. Heat according to the duty cycle formula.

		if (_activelyHeating == false) {
			_lastHeatCycle = now;
			_activelyHeating = true;
			digitalWrite(_pin, HIGH);
		}

	} else if (_targetTemp > 350 || degreesCelsius > 350) {
		// Whoa 'dere. Stop trying to burn the place down.
		// Disables extruder heat until reset button pressed.

		// RampsCerebellum will probably detect this problem when:
		// 1. it sees canExtrude as returning false.
		// 2. it sees getTemp as returning an absurdedly high temperature
		// 	to which it will place the entire machine in a recoverable quarantine state.

		disable(EXTRUDER_EXCEPTION_OVERTEMP);

	} else if (degreesCelsius > _targetTemp) {

		// Turn off the heater, and continue just fine.
		// Figure out the cooling slope, to determine when to reactivate?

		if (_activelyHeating == true) {
			_lastCoolCycle = now;
			digitalWrite(_pin, LOW);
			_activelyHeating = false;
		}

	}

	// Duty cycle understanding, and auto calibration ...how?

}
