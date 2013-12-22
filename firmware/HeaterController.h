/*	* HeaterController.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef HeaterController_h
#define HeaterController_h

#define HEATER_EXCEPTION_OVERTEMP -1
#define FIRE_ALARM_LOCKOUT -2

class HeaterController
{
	public:
		HeaterController() {};
		// (Removed a horrible constructor 2013-12-16)
		void setup(int interfacePin, Thermistor &heaterThermistor);
		void loop(int now);
		
		void setTemp (int degreesCelsius);

		bool isReady ();	// If temp is within +/- 2 degrees, returns true.
		int getTemp (); 

		int enable();	// You need to next setTemp, isReady, setRate
		void disable(int errorCode);	// Turns off the extruder motor, and heat

		void lockout();
	private:
		int _pin;
		int _targetTemp;

		int _errorCode;

		long _nextCheck;

		bool _activelyHeating;

		int _lastHeatCycle;
		int _lastCoolCycle;
		
		Thermistor* _heaterThermistor;

};

#endif