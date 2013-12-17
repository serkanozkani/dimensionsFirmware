/*	* LedIndicator.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef LedIndicator_h
#define LedIndicator_h


class LedIndicator
{
	public:
		LedIndicator() {};
		// (Removed a horrible constructor 2013-12-16)
		void setup(int interfacePin);
		void on();
		void off();

	private:
		int _pin;
		bool _on;

};

#endif