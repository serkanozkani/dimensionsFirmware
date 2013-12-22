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


void Printer::loop(long now)
{
	if (_emergencyStop) {
		emergencyStop();
	}

	if (_actionTestingEndstopX) {
		if (_actionCycles == 2000) {
			_endstop = (Ramps::instance()).getEndstopX();
			Serial.println(_endstop->triggered()?"true":"false");
			_actionCycles = 1;
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
		if (_actionCycles == 0) {
			Serial.println("Testing extruder");
			ExtruderController* extruder = (Ramps::instance()).getExtruderA();
			extruder->setTemp(180);
		} else if (_actionCycles == 2000) {
			ExtruderController* extruder = (Ramps::instance()).getExtruderA();
			int temp = extruder->getTemp();
			Serial.write("Extruder Temp: (deg C) ");
			Serial.println(temp);
			
			if (temp >= 180) {
				extruder->setRate(40);
			} else {
				_actionCycles = 1;
			}
		} else if (_actionCycles == 40000) {
			ExtruderController* extruder = (Ramps::instance()).getExtruderA();
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

// printer.navigate(x, y)
void Printer::navigate(unsigned int x, unsigned int y)
{
	_cartesianController.navigate(x, y);
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

// printer.status()
int Printer::status()
{
	int statusCode = 0;

	if (_error != 0) {
		statusCode = _error;
		return statusCode;
	}

	if (_actionCartesian) {
		statusCode |= CERE_CARTESIAN_ID;
		if (!_cartesianController.ready()) {
			statusCode |= CERE_CARTESIAN_ID << 1;
			statusCode |= _cartesianController.status();
			return statusCode;
		}
	}

	if (_actionSeekingEndstop) {
		statusCode |= 0x1;
	}

	if (_actionTestingEndstopX) {
		statusCode |= 0x2;
	}

	if (_actionTestingMotorX) {
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

	_cartesianController.reset();
	_actionCartesian = false;

	_actionSeekingEndstop = false;
	_actionTestingEndstopX = false;
	_actionTestingMotorX = false;

	if (_actionTestingHeatbed) {
		stopTestingHeatbed();
	}

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