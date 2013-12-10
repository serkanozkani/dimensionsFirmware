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

Ramps rampsInstance;
unsigned long timestamp;
LedIndicator theLed;


/*	* Cache and Tx/Rx:
	*
	*/

unsigned long hash;
unsigned long cycles = 0;
int param1;
int param2;
int param3;

bool pendingRx = false;
char bufferRx[MAX_TX_BUFFER];
word bufferRxPtr = 0;
word bufferRxLen = 0;

/*	* SETUP
	*
	*/

void setup() {
	Serial.begin(9600);

	rampsInstance.setup();
	

	theLed = rampsInstance.getLedIndicator();
	
	theLed.on();

}

/*	* LOOP
	*
	*/


void loop() {
	
	timestamp = millis();

	rampsInstance.loop(timestamp);

	if (cycles == 0) {
		Serial.println(timestamp, DEC);
		Serial.println(pendingRx, DEC);
		if (timestamp > 5000) {
			Serial.println ("Ready for input.");
			pendingRx = true;
		}
	}

	if (pendingRx && Serial.available()){
		char c = Serial.read();

		bufferRx[bufferRxLen] = c;
		bufferRxLen++;
		if (bufferRxLen > MAX_TX_BUFFER) {
			bufferRxLen = 0;
		}
		if (c == '\n') {
			bufferRxLen = 0;
			Serial.println (bufferRx);
		}
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