/*	* Printer.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/


#ifndef Printer_h
#define Printer_h

#include "Ramps.h"

#define CERE_ERROR_CALIBRATE_HALTED_EARLY -1
#define CERE_TEST_COMPLETE 1

#define CERE_DIRECTION_CLOCKWISE true
#define CERE_DIRECTION_ANTI_CLOCKWISE false

#define CERE_MOTOR_X_PRIMARY_DIRECTION CERE_DIRECTION_CLOCKWISE
#define CERE_MOTOR_Y_PRIMARY_DIRECTION CERE_DIRECTION_CLOCKWISE

class Printer
{
  public:
	Printer();

	void setup(const Ramps &rampsInstance);
	void reset();
	void emergencyStop();
	void emergencyStop(int error);

	// Printer.calibrateX()
	void calibrateX();

	// Printer.calibrateY()
	void calibrateY();

	// Printer.calibrateZ()
	void calibrateZ();

	// Printer.testEndstopX()
	void testEndstopX();

	// Printer.testMotorX()
	void testMotorX();

	void cartesian(unsigned int x, unsigned int y);

	void moveX (unsigned int steps, bool direction);

	void moveY (unsigned int steps, bool direction);

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
	bool _actionCalibrateY;
	bool _actionCalibrateZ;
	bool _actionSeekingEndstop;
	bool _startDirection;
	bool _actionTestingEndstopX;
	bool _actionTestingMotorX;
	bool _emergencyStop;

	unsigned int _stepsTodoX;
	unsigned int _stepsTodoY;

	unsigned int _sfoX;
	unsigned int _sfoY;
	unsigned int _sfoZ;

	bool goalTryingToZeroX;

};

#endif