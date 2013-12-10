/*	* HeatbedController.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#include "Thermistor.h"
#include "Arduino.h"


#include "HeatbedController.h"


HeatbedController::HeatbedController ()
{
}

HeatbedController::HeatbedController (int interfacePin, Thermistor heatbedThermistor)
{
	_heatbedThermistor = heatbedThermistor;
	_pin = interfacePin;
}

void HeatbedController::setup()
{
	// No need to setup the extruder motor. Should be done via ramps.cpp
	setTemp(0);
	_errorCode = 0;

	pinMode(_pin, OUTPUT);
	
}

void HeatbedController::disable(int errorCode)
{
	// Stop heating no matter what.
	digitalWrite(_pin, LOW);

	// Handy tip for recovering from an overtemp:
	// disable(0), enable, setTemp, wait, getTemp, setRate and we're back.
	_errorCode = errorCode;
}

int HeatbedController::enable()
{
	if (_errorCode != 0) {
		return _errorCode;
	}

	setTemp(0);

	return 0;
}

void HeatbedController::setTemp(int degreesCelsius)
{
	_targetTemp = degreesCelsius;
}

int HeatbedController::getTemp()
{
	if (_errorCode != 0) {
		return 9999;
	}

	return _heatbedThermistor.getDegreesCelsius();
}

void HeatbedController::loop(int now)
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

		disable(HEATBED_EXCEPTION_OVERTEMP);

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
