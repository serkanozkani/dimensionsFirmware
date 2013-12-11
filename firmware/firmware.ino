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

Ramps rampsInstance;
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
	
	theLed = rampsInstance.getLedIndicator();
	
	theLed.on();

	delay(3000);

	commandParser.write('M');
	commandParser.write('o');
	commandParser.write('o');
	commandParser.write('(');
	commandParser.write('0');
	commandParser.write(',');
	commandParser.write('1');
	commandParser.write(',');
	commandParser.write('1');
	commandParser.write('0');
	commandParser.write(')');
}

/*	* LOOP
	*
	*/


void loop() {
	
	timestamp = millis();

	rampsInstance.loop(timestamp);

	if (cycles == 0) {
		Serial.println(timestamp, DEC);
		if (timestamp > 5000) {
			Serial.println ("Ready for input.");
			readyRx = true;
		}
	}


	if (readyRx && Serial.available()) {
		byte c = Serial.read();
		Serial.println (c);
		//commandParser.write(c);
	
		/*if (commandParser.ready()) {
			readyRx = false;

			Serial.println ("Received command.");
			unsigned long hash;
			int param1;
			int param2;
			int param3;

			//commandParser.read(hash, param1, param2, param3);

			Serial.println (hash);
			Serial.println (param1);
			Serial.println (param2);
			Serial.println (param3);
		}*/
	}

	cycles++;
	if (cycles == 10000) {
		cycles = 0;
	}
}



/*
Looking for the code that manages these PoluluStepper, EndstopSwitch, LedIndicator, Thermistor?

-- Check out the RampsCerebellum which ties together:
	--- the Endstops to the X, Y steppers.
	--- the Thermistors to the heatbed and extruders

	This must run 4 times a second

-- Then, check out the RampsReptilian which handles some simple Fixed Actions
	--- The communication layer to the host system
	--- The visual display patterns that bind the ledIndicator to the boot sequence
	--- The actions necessary to recalibrate the XY positions.
	--- The actions necessary to also recalibrate the Z position.
	--- The ability to draw while extruding according to a list of position+timeOffsets

	This runs continuously, and as predictably smooth as possible.

-- And finally, the RampsGenius which sets up paths for the X, Y steppers with rapid precision
	--- And look at the BezierPathAtomizer, which breaks up a simple/beizer path until it represents
		a set of individual steps, and their timings. (The extruder head needs to extrude at a constant
		rate when working, and the motors do best when they aren't working too hard to accelerate things.)
	--- And look at Infiller which does exactly what you'd expect.

	RampsGenius works to buffer up actions while the printer is running. It is the idle process.


Why don't we see any gcodes? 
-- Gcodes, while simple, don't adequately utilize the full potential of the arduino, 
unnecessarily tax the host system, and make it unreasonably difficult to recover from common
faults. This system is designed to fix those problems and many more. With a firmware-level 
component understanding, the rep-rap less likely to damage itself or be the cause of a fire.
With a 

*/