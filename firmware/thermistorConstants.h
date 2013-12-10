/*	* thermistorConstants.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#include "thermistors.h"

#ifndef thermistorConstants_h
#define thermistorConstants_h


#if (BED_THERMISTOR_TYPE == COMPONENT_100K_THERMISTOR ) || (EXTRUDER_THERMISTOR_TYPE == COMPONENT_100K_THERMISTOR)

#define TABLED_100K_THERMISTOR_COUNT 61
const short TABLED_100K_THERMISTOR [TABLED_100K_THERMISTOR_COUNT][2] = {
	{1000, 300},
	{998, 295},
	{996, 290},
	{995, 285},
	{992, 280},
	{990, 275},
	{988, 270},
	{985, 265},
	{982, 260},
	{979, 255},
	{975, 250},
	{971, 245},
	{967, 240},
	{962, 235},
	{957, 230},
	{952, 225},
	{945, 220},
	{939, 215},
	{931, 210},
	{923, 205},
	{914, 200},
	{903, 195},
	{892, 190},
	{880, 185},
	{867, 180},
	{852, 175},
	{836, 170},
	{818, 165},
	{799, 160},
	{778, 155},
	{755, 150},
	{730, 145},
	{703, 140},
	{675, 135},
	{644, 130},
	{612, 125},
	{578, 120},
	{543, 115},
	{507, 110},
	{470, 105},
	{432, 100},
	{395, 95},
	{358, 90},
	{321, 85},
	{286, 80},
	{253, 75},
	{222, 70},
	{193, 65},
	{166, 60},
	{142, 55},
	{120, 50},
	{101, 45},
	{84, 40},
	{69, 35},
	{57, 30},
	{46, 25},
	{38, 20},
	{30, 15},
	{24, 10},
	{19, 5},
	{15, 0}
};

#endif

#endif