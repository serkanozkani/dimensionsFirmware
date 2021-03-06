/*	* motors.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef motors_h
#define motors_h

/*	* This file is highly variable, 
	* because stepper motors come in many various 
	* forms, spin characteristics, and speed limitations.
	*
	*/

/*	* At this point, if you have not enabled the
	* heat elements it would be safe to test out the motors,
	* and calibrate them.

	* Note: metric resolution and step ratio is calibrated via
	*   software. Our cartesian graph is based on steps, not
	*	mm or any other standard unit.
	*
	* This is designed to minimize floating point calculations 
	*  on the arduino platform, and make it more efficent for
	*  beizer curve plotting.
	*/

#define MOTOR_STEPS_PER_REVOLUTION 3200

#define DELAY_US_PER_HALF_STEP_SLOW 1000
#define DELAY_US_PER_HALF_STEP_NORMAL 250
#define DELAY_US_PER_HALF_STEP_FAST 50

#define CERE_DIRECTION_CLOCKWISE true
#define CERE_DIRECTION_ANTI_CLOCKWISE false

#define CERE_MOTOR_X_PRIMARY_DIRECTION CERE_DIRECTION_CLOCKWISE
#define CERE_MOTOR_Y_PRIMARY_DIRECTION CERE_DIRECTION_CLOCKWISE
#define CERE_MOTOR_Z_PRIMARY_DIRECTION CERE_DIRECTION_ANTI_CLOCKWISE
#define CERE_MOTOR_E0_PRIMARY_DIRECTION CERE_DIRECTION_CLOCKWISE
#define CERE_MOTOR_E1_PRIMARY_DIRECTION CERE_DIRECTION_CLOCKWISE

#endif