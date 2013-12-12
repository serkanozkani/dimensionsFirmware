/*	* RampsCerebellum.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/


#ifndef RampsCerebellum_h
#define RampsCerebellum_h

#include "Ramps.h"

class RampsCerebellum
{
  public:
	RampsCerebellum();

	void setup(const Ramps &rampsInstance);
	void reset();
	void emergencyStop();

	// rampsCerebellum.zeroDimensionX() 2165811644
	void zeroDimensionX();

	// rampsCerebellum.testEndstopX()
	void testEndstopX();

	// rampsCerebellum.testMotorX()
	void testMotorX();

	void loop( unsigned long now);

  private:
	Ramps _rampsInstance;

	// Just one motor and endstop at a time are zeroed.
	// Used to minimize code.
	PoluluStepper _motor;
	EndstopSwitch _endstop;


	unsigned long _actionCycles;
	bool _actionZeroX;
	bool _actionSeekingEndstop;
	bool _actionTestingEndstopX;
	bool _actionTestingMotorX;
	bool _emergencyStop;

	bool goalTryingToZeroX;

};

#endif