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
	_emergencyStop = true;
}


void RampsCerebellum::loop(unsigned long now)
{
	if (_emergencyStop) {
		emergencyStop();
	}

	if (_actionZeroX) {
		_motor = _rampsInstance.getMotorX();
		_endstop = _rampsInstance.getEndstopX();
		_actionSeekingEndstop = true;
	}

	if (_actionTestingEndstopX) {
		if (_actionCycles == 2000) {
			_endstop = _rampsInstance.getEndstopX();
			Serial.println("The endstop state is: ");
			Serial.println(_endstop.triggered()?" -true":"-false");
			_actionCycles = 0;
		}
		_actionCycles++;
	}

	if (_actionTestingMotorX) {

		_motor = _rampsInstance.getMotorX();
			
		if (_actionCycles == 1000) {
			Serial.println("Testing X motor...");
			_motor.enable();
			_motor.fast();
		} else if (_actionCycles == 2000) {
			Serial.println("-Rotating clockwise...");
			_motor.revolve(true);
			Serial.println("-Done.");
		} else if (_actionCycles == 3000) {
			Serial.println("-Rotating counter-clockwise...");
			_motor.revolve(false);
			Serial.println("-Done.");
		} else if (_actionCycles == 4000) {
			emergencyStop();
			Serial.println("Done.");
		}

		_actionCycles++;
	}
}

//rampsCerebellum.reset()
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

}

void RampsCerebellum::zeroDimensionX() {
	emergencyStop();
	_actionZeroX = true;
}

// rampsCerebellum.testEndstopX();
void RampsCerebellum::testEndstopX() {
	emergencyStop();
	_actionTestingEndstopX = true;
}


// rampsCerebellum.testMotorX(); 2911848903
void RampsCerebellum::testMotorX() {
	emergencyStop();
	_actionTestingMotorX = true;
}

void RampsCerebellum::emergencyStop()
{
	_actionCycles = 0;
	_actionZeroX = false;
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
