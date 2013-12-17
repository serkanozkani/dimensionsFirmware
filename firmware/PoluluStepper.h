/*	* PoluluStepper.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-10
	*/

#ifndef PoluluStepper_h
#define PoluluStepper_h


class PoluluStepper
{
	public:
		PoluluStepper() {};
		// (Removed a horrible constructor 2013-12-16)
		void setup(int stepPin, int enablePin, int directionPin, bool startDirection);
		void enable();
		void disable();

		void fast();
		void normal();
		void slow();

		void setDirection (bool forward);

		void revolve (bool forward);

		void rotate( unsigned int numSteps, bool forward);
		void rotate( unsigned int numSteps);

		bool ready();

	private:
		int _stepPin;
		int _enablePin;
		int _directionPin;
		
		bool _startDirection;

		bool _enabled;
		unsigned int _delayPerHalfStep;
};

#endif