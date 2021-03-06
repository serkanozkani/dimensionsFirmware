/*	* EndstopSwitch.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef EndstopSwitch_h
#define EndstopSwitch_h

class EndstopSwitch
{
	public:
		EndstopSwitch() {};
		// (Removed a horrible constructor 2013-12-16)
		void setup(int interfacePin, bool normallyHigh);
		
		bool triggered();

	private:
		int _pin;

		bool _normallyHigh;

};

#endif