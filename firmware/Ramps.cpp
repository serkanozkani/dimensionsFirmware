/*	* Ramps.cpp
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

#include "Ramps.h"

// Configuration
#include "pins.h"
#include "endstops.h"
#include "thermistors.h"

// 

// The components
#include "LedIndicator.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Thermistor.h"


// The "brainlessly simple" controllers
#include "ExtruderController.h"
#include "HeatbedController.h"

Ramps::Ramps()
{
	/*	* Setup of the Ramps Board LED
		*	From: LedIndicator.cpp 
		*	Constructor: (int interfacePin)
		*
		*/

	LedIndicator ledIndicator(LED_PIN);
	_ledIndicator = ledIndicator;

	/*	* Setup of stepper motors
		*	From: PoluluStepper.cpp 
		*	Constructor: (int stepPin, int enablePin, int directionPin)
		*
		*/
	
	PoluluStepper poluluStepperX(X_STEP_PIN, X_ENABLE_PIN, X_DIR_PIN);
	_motorX = poluluStepperX;


	//PoluluStepper poluluStepperY(Y_STEP_PIN, Y_ENABLE_PIN, Y_DIR_PIN);
	//_motorY = poluluStepperY;


	//PoluluStepper poluluStepperZ(Z_STEP_PIN, Z_ENABLE_PIN, Z_DIR_PIN);
	//_motorZ = poluluStepperZ;

	PoluluStepper poluluStepperExtruderA(EXTRUDER_A_STEP_PIN, EXTRUDER_A_ENABLE_PIN, EXTRUDER_A_DIR_PIN);
	_extruderA = poluluStepperExtruderA;

	//PoluluStepper poluluStepperExtruderB(EXTRUDER_B_STEP_PIN, EXTRUDER_B_ENABLE_PIN, EXTRUDER_B_DIR_PIN);
	//_extruderB = poluluStepperExtruderB;

	/*	* Setup of endstop switches.
		*	From: EndstopSwitch.cpp 
		*	Constructor: (int interfacePin, bool normallyHigh)
		*
		*/

	EndstopSwitch endstopX(X_MIN_ENDSTOP_PIN, X_MIN_ENDSTOP_TYPE);
	_endstopX = endstopX;

	EndstopSwitch endstopY(Y_MIN_ENDSTOP_PIN, Y_MIN_ENDSTOP_TYPE);
	_endstopY = endstopY;

	EndstopSwitch endstopZ(Z_MIN_ENDSTOP_PIN, Z_MIN_ENDSTOP_TYPE);
	_endstopZ = endstopZ;

	/*	* Setup of thermistors.
		*	From: Thermistor.cpp 
		*	Constructor: (int interfacePin, int lookupTable)
		*
		*/

	Thermistor thermistorBed(TEMP_BED_PIN, BED_THERMISTOR_TYPE);
	_bedThermistor = thermistorBed;

	Thermistor thermistorExtA(TEMP_EXTRUDER_A_PIN, EXTRUDER_THERMISTOR_TYPE);
	_extruderThermistorA = thermistorExtA;

	//Thermistor thermistorExtB(TEMP_EXTRUDER_B_PIN, EXTRUDER_THERMISTOR_TYPE);
	//_extruderThermistorB = thermistorExtB;

	/*	* Setup of brainlessly simple controllers.
		*	From: ExtruderController.cpp 
		*	Constructor: (PoluluStepper extruderMotor, int HOTEND_A_PIN, Thermistor hotendThermistor)
		*
		*	From: HeatbedController.cpp
		*	Constructor: (int HEATBED_PIN, Thermistor heatbedThermistor)
		*/

	//ExtruderController extruderControllerA(_extruderA, HOTEND_A_PIN, _extruderThermistorA);
	//_extruderControllerA = extruderControllerA;

	//HeatbedController heatbedController(HEATBED_PIN, _bedThermistor);
	//_heatbedController = heatbedController;
}

void Ramps::setup()
{
	/*	* Setup of components
		*/
	_ledIndicator.setup();
	
	_motorX.setup();
	_motorY.setup();
	_motorZ.setup();

	_endstopX.setup();
	_endstopY.setup();
	_endstopZ.setup();

	_bedThermistor.setup();

	_extruderThermistorA.setup();
	//_extruderThermistorB.setup();

	_extruderA.setup();
	//_extruderB.setup();

	/*	* Setup of brainlessly simple controllers
		*/

	_extruderControllerA.setup();

	//_heatbedController.setup();
}

void Ramps::loop(unsigned long ms)
{
	/*	* Update data from components
		*/

	_bedThermistor.loop();
	_extruderThermistorA.loop();

	/*	* Update brainlessly simple controllers
		*/

	_heatbedController.loop(ms);
	_extruderControllerA.loop(ms);

}

LedIndicator Ramps::getLedIndicator()
{
	return _ledIndicator;
}

PoluluStepper Ramps::getMotorX()
{
	return _motorX;
}

PoluluStepper Ramps::getMotorY()
{
	return _motorY;
}

PoluluStepper Ramps::getMotorZ()
{
	return _motorZ;
}

EndstopSwitch Ramps::getEndstopX()
{
	return _endstopX;
}

EndstopSwitch Ramps::getEndstopY()
{
	return _endstopY;
}

EndstopSwitch Ramps::getEndstopZ()
{
	return _endstopZ;
}