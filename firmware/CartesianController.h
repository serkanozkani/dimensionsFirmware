/*	* CartesianController.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-16
	*/

#ifndef CartesianController_h
#define CartesianController_h

#include "LinearMotionController.h"

#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Ramps.h"

class CartesianController
{
	public:
		CartesianController();

		void setup(LinearMotionController *linearMotionControllerX, LinearMotionController *linearMotionControllerY);

		void loop(unsigned long now);

		bool ready();

		void calibrateX ();
		void calibrateY ();

		void navigate(unsigned int stepsX, unsigned int stepsY);

		unsigned int status();
	private:

		LinearMotionController * _linearMotionControllerX;
		LinearMotionController * _linearMotionControllerY;


};

#endif