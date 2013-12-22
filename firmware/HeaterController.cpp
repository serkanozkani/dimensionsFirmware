
/*	* pinoutHeaterController.cpp
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


#include "HeaterController.h"

void HeaterController::setup(int interfacePin, Thermistor &heaterThermistor)
{
	_heaterThermistor = &heaterThermistor;
	_pin = interfacePin;

	// No need to setup the extruder motor. Should be done via ramps.cpp
	setTemp(0);
	_errorCode = 0;

	pinMode(_pin, OUTPUT);
	Serial.write ("Hot item set up on pin: ");
	Serial.println(interfacePin, DEC);

}

void HeaterController::disable(int errorCode)
{
	// Stop heating no matter what.
	digitalWrite(_pin, LOW);

	if (errorCode != 0) {
		Serial.write ("Hot item disabled on pin: ");
	} else if (_errorCode == FIRE_ALARM_LOCKOUT) {
		Serial.write ("Hot item still in lockout: ");
		errorCode = _errorCode;
	}
	Serial.println(_pin, DEC);

	// Handy tip for recovering from an overtemp:
	// disable(0), enable, setTemp, wait, getTemp, setRate and we're back.
	_errorCode = errorCode;
}

int HeaterController::enable()
{
	if (_errorCode != 0) {
		return _errorCode;
	}

	Serial.write ("Hot item enabled on pin: ");
	Serial.println(_pin, DEC);

	setTemp(0);

	return 0;
}

void HeaterController::setTemp(int degreesCelsius)
{
	_targetTemp = degreesCelsius;
	_nextCheck = 0;
}

int HeaterController::getTemp()
{
	if (_errorCode != 0) {
		return 9999;
	}

	return (int) _heaterThermistor->getDegreesCelsius();
}

void HeaterController::loop(int now)
{
	if (_errorCode != 0) {
		digitalWrite(_pin, LOW);
		return;
	}
	if (_nextCheck < now) {
		if (_activelyHeating) {
			_nextCheck = now + 100;
		} else {
			_nextCheck = now + 1000;
		}
	} else {
		return;
	}

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

		disable(HEATER_EXCEPTION_OVERTEMP);

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

void HeaterController::lockout() {
	disable(FIRE_ALARM_LOCKOUT);
}