/*	* firmware.ino
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

/*	* In Arduino IDE
	* How to build:
	*
	*	* Tools->Board->Arduino Mega 2560 or Mega ADK
	*	* Tools->Serial Port-> {Select the proper port}
	*	* In file selection tab (the active one is "firmware"):
	 	 	* Select arrow at far right and edit each of these files:
	 	 	 	* endstops.h 		(Change to normally open if necessary)
	 	 	 	* motors.h  		(Set the calibration of steppers)
	 	 		* thermistors.h 	(If you're using anything but 100k, edit this)
	 	 	 	* ignition.h 		(Read the warning, and follow its instructions)
	*/

#include <avr/pgmspace.h>
#include <math.h>

#include "firmware.h"

#include "Arduino.h"

#include "Ramps.h"

#include "LedIndicator.h"

#include "CommandParser.h"

#include "Printer.h"

#include "ignition.h"

#include "WatchDog.h"

Printer printer;

long timestamp;	// Changed to long from unsigned long, 
// the joys of 2s compliment with subtraction!

CommandParser commandParser;

bool readyRx = false;
bool lockoutShutdown = false;

/*	* SETUP
	*
	*/

void setup() {
	Serial.begin(9600);

	// (Turn off watchdog in setup)
	(Ramps::instance()).setup();

	printer.setup();

	printer.reset();

	readyRx = true;

	// No delay - just go!
	Serial.println("Send \"help()\\n\" for list of commands.");
	Serial.println("ready v0.2.0");

}

/*	* LOOP
	*
	*/


void loop() {
	
	timestamp = millis();

	(Ramps::instance()).loop(timestamp);

	if (lockoutShutdown) {
		// Since ramps doesn't get touched,
		// the thermistors will time-out
		// and the controllers will be ignored.
		// the printer emergencyStop propogates 
		// to all elements on the next ramps loop.

		// emergency stop,
		
		printer.emergencyStop();
		do {
			
			printer.loop(millis());

		} while (printer.status() != 0);

		// propagate stop to ramps heaters
		(Ramps::instance()).allCold();

		// calibrate x,
		printer.calibrateX();
		do {

			printer.loop(millis());

		} while (printer.status() != 0);

		// calibrate y
		printer.calibrateY();
		do {

			printer.loop(millis());

		} while (printer.status() != 0);


		// reset
		printer.reset();
		printer.reset();
		printer.emergencyStop();

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

	if (readyRx && Serial.available()) {
		byte c = Serial.read();
		commandParser.write(c);
	
		if (commandParser.ready()) {
			readyRx = false;

			unsigned long hash;
			unsigned int param1;
			unsigned int param2;
			unsigned int param3;

			commandParser.read(hash, param1, param2, param3);

			switch (hash) {
				case CERE_RESET:
					printer.reset();
					break;
				case CERE_CALIBRATE_X:
					printer.calibrateX();
					break;
				case CERE_CALIBRATE_Y:
					printer.calibrateY();
					break;
				case CERE_CALIBRATE_Z:
					printer.calibrateZ();
					break;
				case CERE_EMERGENCY_STOP:
					printer.emergencyStop();
					break;
				case CERE_TEST_ENDSTOP_X:
					printer.testEndstopX();
					break;
				case CERE_TEST_MOTORS:
					printer.testMotors();
					break;
				case CERE_NAVIGATE:
					printer.navigate(param1, param2);
					break;
				case CERE_STATUS:
					Serial.println(printer.status(), DEC);
					break;
				case CERE_TEST_EXTRUDER:
					if ( !REPT_HOTEND_ENABLED ) {
						Serial.println("The hotend is not enabled.");
						break;
					}
					printer.testExtruder();
					break;
				case CERE_TEST_HEATBED:
					if ( !REPT_HEATBED_ENABLED ) {
						Serial.println("The heatbed is not enabled.");
						break;
					}
					printer.testHeatbed();
					break;
				case CERE_LOCKOUT:
					// Turn off serial comms.
					// Returns motors to home.

					readyRx = false;
					lockoutShutdown = true;
					
					return;
				case CERE_READ_HEATBED:
					// Reads temp from heatbed
					
					Serial.write("HEATBED: (degC) ");
					Serial.println(printer.pollHeatbed());

					break;
				case CERE_HELP:
					Serial.println("printer is capable of: (Enter a line+newline into serial input.)");
					Serial.println("printer.reset() // Blinks led");
					Serial.println("printer.emergencyStop() // Stops any action, clears errors.");
					Serial.println("printer.testEndstopX() // Starts watching sensor");
					Serial.println("printer.testMotorX() // Rotates the motor 1 rev in each dir");
					Serial.println("printer.calibrateX() // Zeroes X dimension (X or Y supported)");
					Serial.println("printer.navigate(x, y) // moves extruder to point");
					Serial.println("help() // this help v0.1.0");
					Serial.println("printer.lockout() // for external fire sensor code");
					Serial.println("printer.testHeatbed() // temp cycles the heatbed briefly");
					Serial.println("printer.testHotend() // place hotend in center of board, one dim at a time, raises 10mm, heats, exrudes, cools");
					Serial.println("printer.status() // int of printer status: 0 = ready");
					Serial.println("printer.pollHeatbed() // float heatbed temperature");
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

			readyRx = true;
		}
	}
	printer.loop(timestamp);
}
