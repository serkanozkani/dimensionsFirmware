/*	* HeatbedController.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef HeatbedController_h
#define HeatbedController_h

#define HEATBED_EXCEPTION_OVERTEMP -1


class HeatbedController
{
	public:
		HeatbedController();
		HeatbedController(int interfacePin, Thermistor heatbedThermistor);
		void setup();
		void loop(int now);
		
		void setTemp (int degreesCelsius);

		bool isReady ();	// If temp is within +/- 2 degrees, returns true.
		int getTemp (); 

		int enable();	// You need to next setTemp, isReady, setRate
		void disable(int errorCode);	// Turns off the extruder motor, and heat

	private:
		int _pin;
		int _targetTemp;

		int _errorCode;

		bool _activelyHeating;

		int _lastHeatCycle;
		int _lastCoolCycle;
		
		Thermistor _heatbedThermistor;

};

#endif