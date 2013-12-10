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
		PoluluStepper();
		PoluluStepper(int stepPin, int enablePin, int directionPin);
		void setup();
		void enable();
		void disable();

		void rotateCW(int numSteps);
		void rotateCCW(int numSteps);
		
		int getStepsRemaining();
		bool step(int);

	private:
		int _stepPin;
		int _enablePin;
		int _directionPin;
		bool _enabled;
		bool _direction;
		bool _directionOld;

		int _stepsRemaining;

};

#endif