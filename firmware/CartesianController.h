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

#include "PoluluStepper.h"
#include "EndstopSwitch.h"
#include "Ramps.h"

class CartesianController
{
	public:
		CartesianController();

		void loop(unsigned long now);

		bool ready();
		
		void zeroX ();
		void zeroY ();

		void calibrateX ();
		void calibrateY ();


		void reset();

	private:

		PoluluStepper * _motorX;
		PoluluStepper * _motorY;
		EndstopSwitch * _endstopX;
		EndstopSwitch * _endstopY;

		unsigned long _actionCycles;

		unsigned int _sfoX;
		unsigned int _sfoY;

		bool _zeroedX;
		bool _zeroedY;

		bool _zeroingX;
		bool _zeroingY;

};

#endif