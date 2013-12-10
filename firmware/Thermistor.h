/*	* Thermistor.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef Thermistor_h
#define Thermistor_h

class Thermistor
{
	public:
		Thermistor();
		Thermistor(int interfacePin, int lookupTable);
		void setup();
		void loop();

		int getDegreesCelsius();

	private:

		int lookupAnalogReading (int encodedState, const short table[][2], int distinctMeasurements);
		
		int _pin;

		int _lookupTable;
		int _degreesCelsius;

};

#endif