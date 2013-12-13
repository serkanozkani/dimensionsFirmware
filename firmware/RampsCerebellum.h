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

#define CERE_ERROR_CALIBRATE_HALTED_EARLY -1
#define CERE_TEST_COMPLETE 1

class RampsCerebellum
{
  public:
	RampsCerebellum();

	void setup(const Ramps &rampsInstance);
	void reset();
	void emergencyStop();
	void emergencyStop(int error);

	// rampsCerebellum.calibrateX() 2165811644
	void calibrateX();

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

	int _error;

	unsigned long _actionCycles;
	bool _actionCalibrateX;
	bool _actionSeekingEndstop;
	bool _actionTestingEndstopX;
	bool _actionTestingMotorX;
	bool _emergencyStop;

	bool goalTryingToZeroX;

};

#endif