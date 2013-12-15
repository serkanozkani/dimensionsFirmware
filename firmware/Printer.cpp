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

// The "brainlessly simple" controllers
#include "ExtruderController.h"
#include "HeatbedController.h"

Printer::Printer()
{

}

void Printer::setup(const Ramps &rampsInstance)
{
	_rampsInstance = rampsInstance;
	_emergencyStop = false;
	_error = 0;
}


void Printer::loop(unsigned long now)
{
	if (_emergencyStop) {
		emergencyStop();
	}

	if (_actionCycles == 0) {
		if (_actionCalibrateX) {
			_motor = _rampsInstance.getMotorX();
			_endstop = _rampsInstance.getEndstopX();
			_startDirection = CERE_MOTOR_X_PRIMARY_DIRECTION;
			_actionSeekingEndstop = true;
		}

		if (_actionCalibrateY) {
			_motor = _rampsInstance.getMotorY();
			_endstop = _rampsInstance.getEndstopY();
			_startDirection = CERE_MOTOR_Y_PRIMARY_DIRECTION;
			_actionSeekingEndstop = true;
		}
	}

	if (_actionTestingEndstopX) {
		if (_actionCycles == 2000) {
			_endstop = _rampsInstance.getEndstopX();
			Serial.println("The endstop state is: ");
			Serial.println(_endstop.triggered()?" -true":"-false");
			Serial.println("(Printer.emergencyStop() to exit)");
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
			_motor.revolve(CERE_DIRECTION_ANTI_CLOCKWISE);
			Serial.println("-Done.");
		} else if (_actionCycles == 3000) {
			Serial.println("-Rotating clockwise...");
			_motor.revolve(CERE_DIRECTION_CLOCKWISE);
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
				_motor.revolve(!_startDirection);
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

		_motor.rotate(10, _startDirection);

		if (_endstop.triggered()) {
			_motor.slow();
			while (_endstop.triggered()) {
				_motor.rotate(1, !_startDirection);
			}
			_actionSeekingEndstop = false;
			if (_actionCalibrateY) {
				_sfoY = 0;
			}
			if (_actionCalibrateX) {
				_sfoX = 0;
			}
		}
	}
	_actionCycles++;
}

// Printer.reset()
void Printer::reset()
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

// printer.calibrateX()
void Printer::calibrateX() {
	emergencyStop();
	_actionCalibrateX = true;
}

// printer.calibrateY()
void Printer::calibrateY() {
	emergencyStop();
	_actionCalibrateY = true;
}

// printer.calibrateZ()
void Printer::calibrateZ() {
	emergencyStop();
	_actionCalibrateZ = true;
}

// printer.cartesian(steps , steps)
void Printer::cartesian(unsigned int x, unsigned int y){
	unsigned int offX;
	bool dirX;
	unsigned int offY;
	bool dirY;

	dirX = (x > _sfoX) ^ CERE_MOTOR_X_PRIMARY_DIRECTION;
	dirY = (y > _sfoY) ^ CERE_MOTOR_Y_PRIMARY_DIRECTION;

	offX = abs(x - _sfoX);
	offY = abs(y - _sfoY);

	moveX (offX, dirX);
	moveY (offY, dirY);
}

//
void Printer::moveX (unsigned int x) {
	_stepsTodoX = x;
	_movingX = true;
}

//
void Printer::moveY (unsigned int y) {
	_stepsTodoY = y;
	_movingY = true;
}

// printer.testEndstopX()
void Printer::testEndstopX() {
	emergencyStop();
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
