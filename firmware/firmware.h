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


// printer.reset() 2549299330
#define CERE_RESET 2549299330

// printer.emergencyStop() 3263447268
#define CERE_EMERGENCY_STOP 3263447268

// printer.calibrateX() 1126058462
#define CERE_CALIBRATE_X 1126058462

// printer.calibrateY() 1126058495
#define CERE_CALIBRATE_Y 1126058495

// printer.calibrateZ() 1126058528
#define CERE_CALIBRATE_Z 1126058528

// printer.testPoluluLibrary() 2129009589
#define CERE_TEST_POLULU 2129009589

// printer.testEndstopX() 1636382900
#define CERE_TEST_ENDSTOP_X 1636382900

// printer.testMotorX() 779790760
#define CERE_TEST_MOTOR_X 779790760

// help()
#define CERE_HELP 261238998

// printer.navigate(x, y) 42975278
#define CERE_NAVIGATE 42975278