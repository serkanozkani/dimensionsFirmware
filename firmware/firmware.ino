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

#include <avr/pgmspace.h>
#include <math.h>

#include "firmware.h"

#include "Arduino.h"

#include "Ramps.h"

#include "LedIndicator.h"

#include "CommandParser.h"

#include "Printer.h"
	
Ramps rampsInstance;
Printer printer;

unsigned long timestamp;
LedIndicator theLed;
CommandParser commandParser;

unsigned long cycles = 0;

bool readyRx = false;

/*	* SETUP
	*
	*/

void setup() {
	Serial.begin(9600);

	rampsInstance.setup();

	printer.setup(rampsInstance);

	printer.reset();

	readyRx = true;

	delay(3000);

	Serial.println("printer is capable of: (Enter a line+newline into serial input.)");
	Serial.println("printer.reset() // Blinks led");
	Serial.println("printer.emergencyStop() // Stops any action");
	Serial.println("printer.testEndstopX() // Starts watching sensor");
	Serial.println("printer.testMotorX() // Rotates the motor 1 rev in each dir");
	Serial.println("printer.calibrateX() // Zeroes X dimension");

}

/*	* LOOP
	*
	*/


void loop() {
	
	timestamp = millis();

	rampsInstance.loop(timestamp);


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
				case CERE_TEST_MOTOR_X:
					printer.testMotorX();
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

	cycles++;
	if (cycles == 10000) {
		cycles = 0;
	}
}