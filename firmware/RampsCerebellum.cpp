/*	* RampsCerebellum.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/


/*

RampsCerebellum.cpp - Library for module-level interface with the ramps 1.4 board.

This bridges together some simple component logic into modules:
	- the ExtruderMotor, ExtruderThermistor and ExtruderHeater into ExtruderController
	- the Heatbed and HeatbedThermistor into HeatbedController
	- the Z axis motors into SliceController
	- the X and Y axises into CartesianController

If you're looking for the code that zeroes out the motors, learns the hotend heating properties, and 
executes a prefab chain of xy movements and extrusions:
go to RampsReptilian.cpp, which conducts these higher-level actions.

If you're looking for the code that plans the chain of xy movements, reacts to exceptions, and communicates with the computer,
check out RampsGenius.cpp

*/


#include "Arduino.h"

#include "RampsCerebellum.h"

// The board
#include "Ramps.h"

// The components
#include "LedIndicator.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Thermistor.h"

// The "brainlessly simple" controllers
#include "ExtruderController.h"
#include "HeatbedController.h"

RampsCerebellum::RampsCerebellum()
{

}

void RampsCerebellum::setup(const Ramps &rampsInstance)
{
	_rampsInstance = rampsInstance;
	_emergencyStop = false;
	_error = 0;
}


void RampsCerebellum::loop(unsigned long now)
{
	if (_emergencyStop) {
		emergencyStop();
	}

	if (_actionCalibrateX) {
		_motor = _rampsInstance.getMotorX();
		_endstop = _rampsInstance.getEndstopX();
		_actionSeekingEndstop = true;
		_actionCalibrateX = false;
	}

	if (_actionTestingEndstopX) {
		if (_actionCycles == 2000) {
			_endstop = _rampsInstance.getEndstopX();
			Serial.println("The endstop state is: ");
			Serial.println(_endstop.triggered()?" -true":"-false");
			Serial.println("(rampsCerebellum.emergencyStop() to exit)");
			_actionCycles = 0;
		}
	}

	if (_actionTestingMotorX) {

		_motor = _rampsInstance.getMotorX();
			
		if (_actionCycles == 1000) {
			Serial.println("Testing X motor...");
			_motor.enable();
			_motor.fast();
		} else if (_actionCycles == 2000) {
			Serial.println("-Rotating counter-clockwise...");
			_motor.revolve(false);
			Serial.println("-Done.");
		} else if (_actionCycles == 3000) {
			Serial.println("-Rotating clockwise...");
			_motor.revolve(true);
			Serial.println("-Done.");
		} else if (_actionCycles == 4000) {
			emergencyStop(CERE_TEST_COMPLETE);
			Serial.println("Done.");
		}
	}

	if (_actionSeekingEndstop) {

		if (_actionCycles == 0) {
			_motor.enable();
			_motor.slow();

			while (_endstop.triggered()) {
				_motor.revolve(false);
			}
		} else if (_actionCycles == 10) {
			_motor.normal();
		} else if (_actionCycles == 100) {
			_motor.fast();
		} else if (_actionCycles == 1200) {
			_motor.normal();
		} else if (_actionCycles == 1300) {
			_motor.slow();
		} else if (_actionCycles == 1400) {
			_motor.disable();
			_actionSeekingEndstop = false;
			emergencyStop(CERE_ERROR_CALIBRATE_HALTED_EARLY);
		}

		_motor.rotate(10, true);

		if (_endstop.triggered()) {
			_motor.slow();
			while (_endstop.triggered()) {
				_motor.rotate(1, false);
			}
			_actionSeekingEndstop = false;
		}
	}
	_actionCycles++;
}

// rampsCerebellum.reset()
void RampsCerebellum::reset()
{
	LedIndicator ledIndicator = _rampsInstance.getLedIndicator();

	ledIndicator.on();
	delay(30);
	ledIndicator.off();
	delay(30);
	ledIndicator.on();
	delay(30);
	ledIndicator.off();

	_error = 0;
}

// rampsCerebellum.calibrateX()
void RampsCerebellum::calibrateX() {
	emergencyStop();
	_actionCalibrateX = true;
}

// rampsCerebellum.testEndstopX()
void RampsCerebellum::testEndstopX() {
	emergencyStop();
	_actionTestingEndstopX = true;
}


// rampsCerebellum.testMotorX()
void RampsCerebellum::testMotorX() {
	emergencyStop();
	_actionTestingMotorX = true;
}


void RampsCerebellum::emergencyStop()
{
	emergencyStop(0);
}

// rampsCerebellum.emergencyStop()
void RampsCerebellum::emergencyStop(int error)
{
	_error = error;

	if (_error) {
		Serial.println("Stop signal received:");
		switch (_error) {
			case CERE_TEST_COMPLETE:
				Serial.println(">>testComplete");
				break;
			case CERE_ERROR_CALIBRATE_HALTED_EARLY:
				Serial.println(">>calibrateReachedEstimatedMechanicalLimit");
				break;
		}
	}

	_actionCycles = 0;
	_actionCalibrateX = false;
	_actionSeekingEndstop = false;
	_actionTestingEndstopX = false;
	_actionTestingMotorX = false;
	_emergencyStop = false;

	_motor = _rampsInstance.getMotorX();
	_motor.disable();

	_motor = _rampsInstance.getMotorY();
	_motor.disable();

	_motor = _rampsInstance.getMotorZ();
	_motor.disable();

	_motor = _rampsInstance.getMotorZ();
	_motor.disable();


}
