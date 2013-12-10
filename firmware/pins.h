/*	* pins.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef pins_h
#define pins_h


// Polulu A4988 Carrier connections
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62

#define EXTRUDER_A_STEP_PIN         26
#define EXTRUDER_A_DIR_PIN          28
#define EXTRUDER_A_ENABLE_PIN       24

#define EXTRUDER_B_STEP_PIN         36
#define EXTRUDER_B_DIR_PIN          34
#define EXTRUDER_B_ENABLE_PIN       30


// Endstops
#define X_MIN_ENDSTOP_PIN           3
#define X_MAX_ENDSTOP_PIN           2
#define Y_MIN_ENDSTOP_PIN          14
#define Y_MAX_ENDSTOP_PIN          15
#define Z_MIN_ENDSTOP_PIN          18
#define Z_MAX_ENDSTOP_PIN          19

// I2C connector (+ - 20 21 marking by endstops)
//      I2C_PIN_1	Vcc 5v
//      I2C_PIN_2   GND
#define I2C_PIN_3	20
#define I2C_PIN_4	21

// AUX 3 connector
// Usually used for SDRAMPS, so named as such.
//		SDR_PIN_1	Vcc 5v
#define SDR_PIN_2	49		// SD_CARD_DETECT
#define SDR_PIN_3	50		// MISO
#define SDR_PIN_4	51		// MOSI
#define SDR_PIN_5	52		// SCK
#define SDR_PIN_6	53		// SDSS
//		SDR_PIN_7	GND
//		SDR_PIN_8	NC

// LED
#define LED_PIN            13

// PS-ON
// (3 pin header located near 12v aux,
//  and the power supply and big flat fuses)
#define PS_ON_PIN          12

// These are the N-channel switched 12V powered outputs 
#define HOTEND_A_PIN       10	// (Extruder)
#define HOTEND_B_PIN       9	// (Extruder)
#define HEATBED_PIN        8	// (Heatbed)


#define TEMP_BED_PIN 			13   // ANALOG NUMBERING / T0
#define TEMP_EXTRUDER_A_PIN 	14   // ANALOG NUMBERING / T1
#define TEMP_EXTRUDER_B_PIN 	15   // ANALOG NUMBERING / T2


#endif