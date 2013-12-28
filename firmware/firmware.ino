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

#include "Application.h"

#include "Ramps.h"

#include "LedIndicator.h"

#include "CommandParser.h"

#include "Printer.h"

#include "ignition.h"

#include "WatchDog.h"

Printer printer;

Application application;

long _timestamp;	// Changed to long from unsigned long, 
// the joys of 2s compliment with subtraction!

CommandParser commandParser;

bool _readyRx = false;
bool _lockoutShutdown = false;

/*	* SETUP
	*
	*/

void setup() {
	application.setup(&printer, &commandParser);
}

/*	* LOOP
	*
	*/


void loop() {
	application.loop(millis());
}
