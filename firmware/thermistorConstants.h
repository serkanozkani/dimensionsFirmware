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
	{1009,0},
	{1005,5},
	{1000,10},
	{994,15},
	{986,20},
	{978,25},
	{967,30},
	{955,35},
	{940,40},
	{923,45},
	{904,50},
	{882,55},
	{858,60},
	{831,65},
	{802,70},
	{771,75},
	{738,80},
	{703,85},
	{666,90},
	{629,95},
	{592,100},
	{554,105},
	{517,110},
	{481,115},
	{446,120},
	{412,125},
	{380,130},
	{349,135},
	{321,140},
	{294,145},
	{269,150},
	{246,155},
	{225,160},
	{206,165},
	{188,170},
	{172,175},
	{157,180},
	{144,185},
	{132,190},
	{121,195},
	{110,200},
	{101,205},
	{93,210},
	{85,215},
	{79,220},
	{72,225},
	{67,230},
	{62,235},
	{57,240},
	{53,245},
	{49,250},
	{45,255},
	{42,260},
	{39,265},
	{36,270},
	{34,275},
	{32,280},
	{29,285},
	{28,290},
	{26,295},
	{24,300}
};

#endif

#endif