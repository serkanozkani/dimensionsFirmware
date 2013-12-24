/*	* LinearMotionController.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-23
	*/

#ifndef LinearMotionController_h
#define LinearMotionController_h

#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Ramps.h"

class LinearMotionController
{
	public:
		LinearMotionController();

		void setup (PoluluStepper *motor, EndstopSwitch *endstopSwitch);

		void calibrate();
		void singleRevolution();
		unsigned int moveTo(unsigned int stepsFromOrigin);

		void loop(unsigned long now);

		unsigned int status();

		bool ready();
		void reset();

	private:
		void _done();

		PoluluStepper * _motor;
		EndstopSwitch * _endstop;

		unsigned long _actionCycles;

		int _stepsFromOrigin;
		unsigned int _stepsFromTarget;

		bool _zeroed;

		bool _zeroing;

		bool _movingToTarget;
		bool _startingDirection;

};

#endif