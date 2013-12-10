/*	* Thermistor.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#include "Thermistor.h"
#include "Arduino.h"


#include "thermistorConstants.h"

Thermistor::Thermistor()
{
	_pin = -1;
	_lookupTable = -1;
	_degreesCelsius = 9999;
}

Thermistor::Thermistor(int interfacePin, int lookupTable)
{
	_pin = interfacePin;
	_lookupTable = lookupTable;
	_degreesCelsius = 9999;
}

void Thermistor::setup()
{
	// No need to set up, since analog pins default to input.
}


int Thermistor::getDegreesCelsius()
{
	// Simply gets the last state as reported from the loop.

	return _degreesCelsius;
}

int Thermistor::lookupAnalogReading (int encodedState, const short table[][2], int distinctMeasurements)
{

	int estimatedTemp = 9999;

	// Now convert the temperature, using our handy lookup table.
	byte i;
	for (i = distinctMeasurements; i--;) {
		if (encodedState < table[i][0]) {
			// i  table{[0], [1]}
			// 30 {755, 150},
			// encodedState = 741, i = 30
			// 31	{730, 145}

			// Now take the slope of the line, and estimate where T resides.

			estimatedTemp = table[i][1] + (encodedState - table[i][0]) *
							(table[i+1][1] - table[i][1] / 
							 table[i+1][0] - table[i][0]);

			return estimatedTemp;
		}
	}

	return estimatedTemp;
}

void Thermistor::loop()
{
	int encodedState = analogRead(_pin);
	

	if (_lookupTable == COMPONENT_100K_THERMISTOR) {
		_degreesCelsius = lookupAnalogReading(encodedState, TABLED_100K_THERMISTOR, TABLED_100K_THERMISTOR_COUNT);
	} else {
		return;
	}
}
