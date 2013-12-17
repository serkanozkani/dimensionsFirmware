/*	* Printer.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/
#include "Arduino.h"

#include "Printer.h"

// The board
#include "Ramps.h"

// The components
#include "LedIndicator.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Thermistor.h"

// Some Config
#include "motors.h"

// The "brainlessly simple" controllers
#include "ExtruderController.h"
#include "HeatbedController.h"

// The more complex controller
#include "CartesianController.h"

Printer::Printer()
{

}

void Printer::setup()
{
	_cartesianController.reset();

	_emergencyStop = false;
	_error = 0;

}


void Printer::loop(unsigned long now)
{
	if (_emergencyStop) {
		emergencyStop();
	}

	if (_actionTestingEndstopX) {
		if (_actionCycles == 2000) {
			_endstop = (Ramps::instance()).getEndstopX();
			Serial.println(_endstop->triggered()?"true":"false");
			_actionCycles = 0;
		}
	}


	if (_actionTestingMotorX) {

		_motor = (Ramps::instance()).getMotorX();
			
		if (_actionCycles == 0) {
			_motor->enable();
			_motor->fast();
		} else if (_actionCycles == 1000) {
			Serial.println("-Rotating counter-clockwise...");
			_motor->revolve(CERE_DIRECTION_ANTI_CLOCKWISE);
			Serial.println("-Done.");
		} else if (_actionCycles == 2000) {
			Serial.println("-Rotating clockwise...");
			_motor->revolve(CERE_DIRECTION_CLOCKWISE);
			Serial.println("-Done.");
		} else if (_actionCycles == 3000) {
			emergencyStop(CERE_TEST_COMPLETE);
			Serial.println("ready");
		}
	}

	if (_actionCartesian) {
		_cartesianController.loop(now);

		if (_cartesianController.ready()) {
			_actionCartesian = false;
			Serial.println("ready");
		}
	}

	_actionCycles++;
}

// Printer.reset()
void Printer::reset()
{
	LedIndicator *ledIndicator = (Ramps::instance()).getLedIndicator();

	ledIndicator->on();
	delay(30);
	ledIndicator->off();
	delay(30);
	ledIndicator->on();
	delay(30);
	ledIndicator->off();

	_cartesianController.reset();

	_error = 0;

	Serial.println("ready");
}

// printer.calibrateX()
void Printer::calibrateX() {
	_cartesianController.calibrateX();
	_actionCartesian = true;
}

// printer.calibrateY()
void Printer::calibrateY() {
	_cartesianController.calibrateY();
	_actionCartesian = true;
}

// printer.calibrateZ()
void Printer::calibrateZ() {
	//emergencyStop();
	//_actionCalibrateZ = true;
}

// printer.testEndstopX()
void Printer::testEndstopX() {
	emergencyStop();
	Serial.println("Send \"printer.emergencyStop()\\n\" to stop polling.");
	_actionTestingEndstopX = true;
}

// printer.testMotorX()
void Printer::testMotorX() {
	emergencyStop();
	_actionTestingMotorX = true;
}

// printer.emergencyStop()
void Printer::emergencyStop()
{
	emergencyStop(0);
}

void Printer::emergencyStop(int error)
{
	_error = error;

	if (_error) {
		Serial.println("Stop signal received:");
		switch (_error) {
			case CERE_TEST_COMPLETE:
				Serial.println(">>testComplete");
				break;
		}
	}

	_actionCycles = 0;

	_cartesianController.reset();
	_actionCartesian = false;

	_actionSeekingEndstop = false;
	_actionTestingEndstopX = false;
	_actionTestingMotorX = false;


	_emergencyStop = false;

	_motor = (Ramps::instance()).getMotorX();
	_motor->disable();

	_motor = (Ramps::instance()).getMotorY();
	_motor->disable();

	_motor = (Ramps::instance()).getMotorZ();
	_motor->disable();

	_motor = (Ramps::instance()).getMotorZ();
	_motor->disable();


	Serial.println("ready");

}
