/*	* ExtruderController.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef ExtruderController_h
#define ExtruderController_h

#define EXTRUDER_EXCEPTION_OVERTEMP -1

#include "HeaterController.h"

class ExtruderController
{
	public:
		ExtruderController() {};
		// (Removed a horrible constructor 2013-12-16)
		void setup(PoluluStepper &extruderMotor, HeaterController &heaterController);
		
		void loop(long now);
		
		void setRate (int mmHrRate);
		void setTemp (int degreesCelsius);

		bool isReady ();	// If temp is within +/- 2 degrees, returns true.
		int getTemp (); 

		int enable();	// You need to next setTemp, isReady, setRate
		void disable(int errorCode);	// Turns off the extruder motor, and heat

	private:

		int _mmHrRate;

		int _errorCode;

		long _nextStepperCycle;

		PoluluStepper* _extruderMotor;
		HeaterController* _heaterController;

};

#endif