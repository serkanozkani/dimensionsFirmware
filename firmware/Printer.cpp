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
#include "HeaterController.h"

// The more complex controllers
#include "CartesianController.h"
#include "LinearMotionController.h"

Printer::Printer()
{

}

void Printer::setup()
{

	_emergencyStop = false;
	_error = 0;

	_linearMotionControllerX.setup(
		(Ramps::instance()).getMotorX(),
		(Ramps::instance()).getEndstopX());

	_linearMotionControllerY.setup(
		(Ramps::instance()).getMotorY(),
		(Ramps::instance()).getEndstopY());

	_linearMotionControllerZ.setup(
		(Ramps::instance()).getMotorZ(),
		(Ramps::instance()).getEndstopZ());


	_cartesianController.setup(&_linearMotionControllerX, &_linearMotionControllerY);
}


void Printer::loop(unsigned long now)
{
	if (_emergencyStop) {
		emergencyStop();
	}

	_linearMotionControllerX.loop(now);
	_linearMotionControllerY.loop(now);
	_linearMotionControllerZ.loop(now);

	if (_actionTestingEndstopX) {
		if (_actionCycles == 2000) {
			EndstopSwitch * _endstop = (Ramps::instance()).getEndstopX();
			Serial.println(_endstop->triggered()?"true":"false");
			_actionCycles = 1;
		}
	}

	if (_actionTestingHeatbed) {
		if (_actionCycles == 0) {
			Serial.println("Testing heatbed");
			(Ramps::instance()).setHeatbedTemp(60);
		} else if (_actionCycles == 2000) {
			int temp = (Ramps::instance()).pollHeatbed();
			Serial.write("Heatbed Temp: (deg C) ");
			Serial.println(temp);
			
			if (temp >= 60) {
				stopTestingHeatbed();
				Serial.println("ready");
			}
			_actionCycles = 1;

		}
	}

	if (_actionTestingExtruder) {
		
		ExtruderController* extruder;

		if (_actionCycles == 0 || _actionCycles == 2000 || _actionCycles == 40000) {
			extruder = (Ramps::instance()).getExtruderA();
		}

		if (_actionCycles == 0) {
			Serial.println("Testing extruder");
			extruder->setTemp(180);
		} else if (_actionCycles == 2000) {
			int temp = extruder->getTemp();
			Serial.write("Extruder Temp: (deg C) ");
			Serial.println(temp);
			
			if (temp >= 180) {
				extruder->setRate(40);
			} else {
				_actionCycles = 1;
			}
		} else if (_actionCycles == 40000) {
			_actionTestingExtruder = false;
			extruder->setRate(0);
			extruder->setTemp(0);
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

	_error = 0;

	Serial.println("ready");
}

// printer.calibrateX()
void Printer::calibrateX() {
	_linearMotionControllerX.calibrate();
}

// printer.calibrateY()
void Printer::calibrateY() {
	_linearMotionControllerY.calibrate();
}

// printer.navigate(x, y)
void Printer::navigate(unsigned int x, unsigned int y)
{
	_cartesianController.navigate(x, y);
}

// printer.calibrateZ()
void Printer::calibrateZ() {
	_linearMotionControllerZ.calibrate();
}

// printer.testEndstopX()
void Printer::testEndstopX() {
	emergencyStop();
	Serial.println("Send \"printer.emergencyStop()\\n\" to stop polling.");
	_actionTestingEndstopX = true;
}

// printer.testMotors()
void Printer::testMotors() {
	emergencyStop();
	_linearMotionControllerX.singleRevolution();
	_linearMotionControllerY.singleRevolution();
	_linearMotionControllerZ.singleRevolution();

	_linearMotionControllerX.calibrate();
	_linearMotionControllerY.calibrate();

}

// printer.status()
int Printer::status()
{
	int statusCode = 0;

	if (_error != 0) {
		statusCode = _error;
		return statusCode;
	}

	if (_actionSeekingEndstop) {
		statusCode |= 0x1;
	}

	if (_actionTestingEndstopX) {
		statusCode |= 0x2;
	}

	if (_actionTestingMotors) {
		statusCode |= 0x4;
	}

	if (_actionTestingHeatbed) {
		statusCode |= 0x8;
	}
	
	if (_actionTestingExtruder) {
		statusCode |= 0x10;
	}

	return statusCode;
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

	_actionSeekingEndstop = false;
	_actionTestingEndstopX = false;
	_actionTestingMotors = false;

	if (_actionTestingHeatbed) {
		stopTestingHeatbed();
	}

	_emergencyStop = false;

	(Ramps::instance()).getMotorX()->disable();

	(Ramps::instance()).getMotorY()->disable();

	(Ramps::instance()).getMotorZ()->disable();

	(Ramps::instance()).getMotorZ()->disable();


	Serial.println("ready");

}


// printer.pollHeatbed()
float Printer::pollHeatbed ()
{
	Thermistor * heatbedTherm  = (Ramps::instance()).getHeatbedThermistor();
	return heatbedTherm->getDegreesCelsius();
}

void Printer::testHeatbed() 
{
	_actionTestingHeatbed = true;
	_actionCycles = 0;
}

void Printer::testExtruder() 
{
	_actionTestingExtruder = true;
	_actionCycles = 0;
}

void Printer::stopTestingHeatbed()
{
	(Ramps::instance()).setHeatbedTemp(0);
	_actionTestingHeatbed = false;
}