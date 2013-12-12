/*	* firmware.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#include "Arduino.h"

#define MAX_TX_BUFFER 1024


// rampsCerebellum.reset()
#define CERE_RESET 288131393

// rampsCerebellum.emergencyStop()
#define CERE_EMERGENCY_STOP 820371619

// rampsCerebellum.zeroDimensionX()
#define CERE_ZERO_X 2165811644

// rampsCerebellum.testPoluluLibrary() 2155840244
#define CERE_TEST_POLULU 2155840244

// rampsCerebellum.testEndstopX() 4165360787
#define CERE_TEST_ENDSTOP_X 4165360787


// rampsCerebellum.testMotorX(); 2911848903
#define CERE_TEST_MOTOR_X 2911848903