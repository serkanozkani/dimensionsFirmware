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
#include "CartesianController.h"

#define CERE_TEST_COMPLETE 1

#define CERE_CARTESIAN_ID 0x100

class Printer
{
  public:
	Printer();

	void setup();
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

	void moveX (unsigned int stepX);
	void moveY (unsigned int stepY);
	void navigate(unsigned int x, unsigned int y);

	// Printer.pollHeatbed()
	float pollHeatbed();

	void loop( long now);

	int status();

	void testHeatbed();
	void testExtruder();
	void stopTestingHeatbed();

  private:
	CartesianController _cartesianController;

	// Just one motor and endstop at a time are zeroed.
	// Used to minimize code.
	PoluluStepper * _motor;
	EndstopSwitch * _endstop;

	int _error;

	unsigned long _actionCycles;
	bool _actionCartesian;

	bool _actionSeekingEndstop;
	bool _startDirection;
	bool _actionTestingEndstopX;
	bool _actionTestingMotorX;
	bool _actionTestingHeatbed;
	bool _actionTestingExtruder;

	bool _emergencyStop;



};

#endif