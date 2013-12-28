/*	* Application.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-27
	*/

#include "Arduino.h"

#include "Application.h"

#include "Ramps.h"

#include "LedIndicator.h"

#include "CommandParser.h"

#include "Printer.h"

#include "ignition.h"

void Application::setup(Printer * printer, CommandParser * commandParser) {

	_printer = printer;
	_commandParser = commandParser;

	Serial.begin(9600);

	// (Turn off watchdog in setup)
	(Ramps::instance()).setup();

	_printer->setup();

	_printer->reset();

	_readyRx = true;

	// No delay - just go!
	Serial.println("Send \"help()\\n\" for list of commands.");
	Serial.println("ready v0.2.0");

}

void Application::lockout () {
	// Since ramps doesn't get touched,
	// the thermistors will time-out
	// and the controllers will be ignored.
	// the _printer->emergencyStop propogates 
	// to all elements on the next ramps loop.

	// emergency stop,
	
	_printer->emergencyStop();
	do {
		
		_printer->loop(millis());

	} while (_printer->status() != 0);

	// propagate stop to ramps heaters
	(Ramps::instance()).allCold();

	// calibrate x,
	_printer->calibrateX();
	do {

		_printer->loop(millis());

	} while (_printer->status() != 0);

	// calibrate y
	_printer->calibrateY();
	do {

		_printer->loop(millis());

	} while (_printer->status() != 0);


	// reset
	_printer->reset();
	_printer->reset();
	_printer->emergencyStop();

	Serial.println("Entered lockout mode. Button reset or reboot to continue.");

	LedIndicator * ledIndicator = (Ramps::instance()).getLedIndicator();
	while (true) {
		delay(3000);
		ledIndicator->on();
		delay(100);
		ledIndicator->off();
		delay(100);
		ledIndicator->on();
		delay(100);
		ledIndicator->off();
	}

	return;
}

void Application::loop (unsigned long timestamp) {

	_timestamp = timestamp;

	(Ramps::instance()).loop(_timestamp);

	if (_activeAction == ApplicationCommand::LOCKOUT) {
		return lockout();
	}
		
	if (_readyRx && Serial.available()) {
		byte c = Serial.read();
		_commandParser->write(c);
	
		if (_commandParser->ready()) {
			_readyRx = false;

			unsigned long hash;
			unsigned int param1;
			unsigned int param2;
			unsigned int param3;

			_commandParser->read(hash, param1, param2, param3);

			switch (hash) {
				case ApplicationCommand::RESET:
					_printer->reset();
					break;
				case ApplicationCommand::CALIBRATE_X:
					_printer->calibrateX();
					break;
				case ApplicationCommand::CALIBRATE_Y:
					_printer->calibrateY();
					break;
				case ApplicationCommand::CALIBRATE_Z:
					_printer->calibrateZ();
					break;
				case ApplicationCommand::EMERGENCY_STOP:
					_printer->emergencyStop();
					break;
				case ApplicationCommand::TEST_ENDSTOP_X:
					_printer->testEndstopX();
					break;
				case ApplicationCommand::TEST_MOTORS:
					_printer->testMotors();
					break;
				case ApplicationCommand::NAVIGATE:
					_printer->navigate(param1, param2);
					break;
				case ApplicationCommand::STATUS:
					Serial.println(_printer->status(), DEC);
					break;
				case ApplicationCommand::TEST_EXTRUDER:
					if ( !REPT_HOTEND_ENABLED ) {
						Serial.println("The hotend is not enabled.");
						break;
					}
					_printer->testExtruder();
					break;
				case ApplicationCommand::TEST_HEATBED:
					if ( !REPT_HEATBED_ENABLED ) {
						Serial.println("The heatbed is not enabled.");
						break;
					}
					_printer->testHeatbed();
					break;
				case ApplicationCommand::LOCKOUT:
					// Turn off serial comms.
					// Returns motors to home.

					_readyRx = false;
					_activeAction = hash;
					
					return;
				case ApplicationCommand::READ_HEATBED:
					// Reads temp from heatbed
					
					Serial.write("HEATBED: (degC) ");
					Serial.println(_printer->pollHeatbed());

					break;
				case ApplicationCommand::HELP:
					Serial.println("_printer->is capable of: (Enter a line+newline into serial input.)");
					Serial.println("_printer->reset() // Blinks led");
					Serial.println("_printer->emergencyStop() // Stops any action, clears errors.");
					Serial.println("_printer->testEndstopX() // Starts watching sensor");
					Serial.println("_printer->testMotorX() // Rotates the motor 1 rev in each dir");
					Serial.println("_printer->calibrateX() // Zeroes X dimension (X or Y supported)");
					Serial.println("_printer->navigate(x, y) // moves extruder to point");
					Serial.println("help() // this help v0.1.0");
					Serial.println("_printer->lockout() // for external fire sensor code");
					Serial.println("_printer->testHeatbed() // temp cycles the heatbed briefly");
					Serial.println("_printer->testHotend() // place hotend in center of board, one dim at a time, raises 10mm, heats, exrudes, cools");
					Serial.println("_printer->status() // int of _printer->status: 0 = ready");
					Serial.println("_printer->pollHeatbed() // float heatbed temperature");
					Serial.println("help() // this help v0.2.0");
					break;
				default:
					Serial.println (hash);
					Serial.println ("1: ");
					Serial.println (param1, DEC);
					Serial.println ("2: ");
					Serial.println (param2, DEC);
					Serial.println ("3: ");
					Serial.println (param3, DEC);
					
					break;
			}

			_readyRx = true;
		}
	}
	_printer->loop(_timestamp);
}