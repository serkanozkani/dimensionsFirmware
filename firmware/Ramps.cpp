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

#include "Ramps.h"

// Configuration
#include "pins.h"
#include "endstops.h"
#include "thermistors.h"
#include "motors.h"

// The components
#include "LedIndicator.h"
#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Thermistor.h"

// The potentially dangerous components, shoehorned into modules.
//#include "ExtruderController.h"
#include "HeatbedController.h"

/*	* Ramps became a singleton. Why?
	*
	*	- There is only going to ever be one instance of each component in memory.
	*	- Whenever ramps got copied, really big problems started happening.
	*		I.e. A motor would not behave with the same speed across the entire codebase.
	*	- In order to preserve the idea of keeping all the components in one handy class,
	*		this seemed the best way to go about things, instead of polluting the global namespace
	*		with a bunch of assorted components.
	*	- By making this a singleton, I can ensure components are properly grouped into their
	*		higher level modules wihout giving access to potentially dangerous individual parts.
	*		I.e. Locking out direct access to the heatbed switch, 
	*			when it should be only indirectly controlled by
	*			setting the target temperature of its controller.
	*		It is perhaps safer, because the heatbed controller can be accessed, but the actual 
	*			heatbed switches cannot be. Code that accesses the control pin directly would
	*			be ugly and obvious, reducing the temptation for hacky shortcuts.
	*/

void Ramps::setup()
{
	/*	* Setup of the Ramps Board LED
		*	From: LedIndicator.cpp 
		*	Setup: (int interfacePin)
		*
		*/

	_ledIndicator.setup(LED_PIN);

	/*	* Setup of stepper motors
		*	From: PoluluStepper.cpp 
		*	Setup: (int stepPin, int enablePin, int directionPin)
		*
		*/
	
	_motorX.setup(X_STEP_PIN, X_ENABLE_PIN, X_DIR_PIN, CERE_MOTOR_X_PRIMARY_DIRECTION);
	
	_motorY.setup(Y_STEP_PIN, Y_ENABLE_PIN, Y_DIR_PIN, CERE_MOTOR_Y_PRIMARY_DIRECTION);

	_motorZ.setup(Z_STEP_PIN, Z_ENABLE_PIN, Z_DIR_PIN, CERE_MOTOR_Z_PRIMARY_DIRECTION);
	
	_extruderA.setup(EXTRUDER_A_STEP_PIN, EXTRUDER_A_ENABLE_PIN, EXTRUDER_A_DIR_PIN, CERE_MOTOR_E0_PRIMARY_DIRECTION);

	//_extruderB.setup(EXTRUDER_B_STEP_PIN, EXTRUDER_B_ENABLE_PIN, EXTRUDER_B_DIR_PIN);
	
	/*	* Setup of endstop switches.
		*	From: EndstopSwitch.cpp 
		*	Setup: (int interfacePin, bool normallyHigh)
		*
		*/

	_endstopX.setup(X_MIN_ENDSTOP_PIN, X_MIN_ENDSTOP_TYPE);

	_endstopY.setup(Y_MIN_ENDSTOP_PIN, Y_MIN_ENDSTOP_TYPE);
	
	_endstopZ.setup(Z_MIN_ENDSTOP_PIN, Z_MIN_ENDSTOP_TYPE);

	/*	* Setup of thermistors.
		*	From: Thermistor.cpp 
		*	Setup: (int interfacePin, int lookupTable)
		*
		*/

	_bedThermistor.setup(TEMP_BED_PIN, BED_THERMISTOR_TYPE);

	_extruderThermistorA.setup(TEMP_EXTRUDER_A_PIN, EXTRUDER_THERMISTOR_TYPE);

	//_extruderThermistorB.setup(TEMP_EXTRUDER_B_PIN, EXTRUDER_THERMISTOR_TYPE);

	/*	* Setup of dangerous heater components, shoehorned into controllers.
		*	From: ExtruderController.cpp, HeatbedController.h
		*
		*/

	_heatbedController.setup(HEATBED_PIN, _bedThermistor);
}

void Ramps::loop(long ms)
{
	/*	* Update data from components
		*/

	_bedThermistor.loop();
	_extruderThermistorA.loop();

	/*	* Update the 'hot' controllers
		*
		* Stopping fires from happening, one cycle at a time...
		*
		*/

	// To be defined and tested first...
	
	_heatbedController.loop(ms);
	
	//_extruderControllerA.loop(ms);

}

void Ramps::allCold()
{
	_heatbedController.lockout();
	//_extruderController.lockout();
}

LedIndicator* Ramps::getLedIndicator()
{
	return &_ledIndicator;
}

PoluluStepper* Ramps::getMotorX()
{
	return &_motorX;
}

PoluluStepper* Ramps::getMotorY()
{
	return &_motorY;
}

PoluluStepper* Ramps::getMotorZ()
{
	return &_motorZ;
}

EndstopSwitch* Ramps::getEndstopX()
{
	return &_endstopX;
}

EndstopSwitch* Ramps::getEndstopY()
{
	return &_endstopY;
}

EndstopSwitch* Ramps::getEndstopZ()
{
	return &_endstopZ;
}

Thermistor* Ramps::getHeatbedThermistor()
{
	return &_bedThermistor;
}