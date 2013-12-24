/*	* CartesianController.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-16
	*/


#include "CartesianController.h"

#include "LinearMotionController.h"

#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Arduino.h"

CartesianController::CartesianController()
{
}

void CartesianController::loop(unsigned long now) {
	return;
}

bool CartesianController::ready() {
	if (!_linearMotionControllerX->ready()) return false;
	if (!_linearMotionControllerY->ready()) return false;

	return true;
}

void CartesianController::setup(LinearMotionController *linearMotionControllerX, LinearMotionController *linearMotionControllerY)
{
	_linearMotionControllerX = linearMotionControllerX;
	_linearMotionControllerY = linearMotionControllerY;
}

void CartesianController::navigate(unsigned int stepsX, unsigned int stepsY)
{
	_linearMotionControllerX->moveTo(stepsX);
	_linearMotionControllerY->moveTo(stepsY);
}

void CartesianController::calibrateX ()
{
	_linearMotionControllerX->calibrate();
}

void CartesianController::calibrateY ()
{
	_linearMotionControllerY->calibrate();
}
