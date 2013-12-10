/*	* RampsCerebellum.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/


/*

RampsCerebellum.cpp - Library for module-level interface with the ramps 1.4 board.

This bridges together some simple component logic into modules:
	- the ExtruderMotor, ExtruderThermistor and ExtruderHeater into ExtruderController
	- the Heatbed and HeatbedThermistor into HeatbedController
	- the Z axis motors into SliceController
	- the X and Y axises into CartesianController

If you're looking for the code that zeroes out the motors, learns the hotend heating properties, and 
executes a prefab chain of xy movements and extrusions:
go to RampsReptilian.cpp, which conducts these higher-level actions.

If you're looking for the code that plans the chain of xy movements, reacts to exceptions, and communicates with the computer,
check out RampsGenius.cpp

*/


#include "Arduino.h"

#include "RampsCerebellum.h"

// The components
#include "LedIndicator.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Thermistor.h"

// The "brainlessly simple" controllers
#include "ExtruderController.h"
#include "HeatbedController.h"


RampsCerebellum::RampsCerebellum()
{

}

void RampsCerebellum::setup()
{

}

void RampsCerebellum::reset()
{

}

void RampsCerebellum::emergencyStop()
{

}
