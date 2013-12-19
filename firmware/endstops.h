/*	* endstops.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

/*	*	You'll need to check if your switches are normally open, or normally closed.
	*	
	*	EndstopSwitch utilizes the AtMega chip's pullup resistors.
	*	
	*	Switch
	*
	*    vcc         _ATMEGA_
	*     | +       | vcc     pullup
	*     -         |  |   /  resistor
	*    ---    / _ | [ ]   
	* gnd |____/  |_|__|__data
	*    
	*   Normally open:
	*		Use SWITCH_TYPE_NORMALLY_HIGH
	*/

/*	*	Switch
	*
	*    vcc         _ATMEGA_
	*     | +       | vcc     pullup
	*     -         |  |   /  resistor
	*    ---    _+= | [ ]   
	* gnd |____// |_|__|__data
	*    
	*   Normally closed:
	*		Use SWITCH_TYPE_NORMALLY_LOW
	*/


#ifndef endstops_h
#define endstops_h

#define SWITCH_TYPE_NORMALLY_HIGH true
#define SWITCH_TYPE_NORMALLY_LOW false

#define X_MIN_ENDSTOP_TYPE       SWITCH_TYPE_NORMALLY_HIGH
#define Y_MIN_ENDSTOP_TYPE       SWITCH_TYPE_NORMALLY_HIGH
#define Z_MIN_ENDSTOP_TYPE       SWITCH_TYPE_NORMALLY_HIGH

#endif