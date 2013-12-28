/*	* Application.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-27
	*/


#ifndef Application_h
#define Application_h

#define MAX_TX_BUFFER 1024

#include "CommandParser.h"
#include "Printer.h"


class Application
{
	public:
		Application() {
			_readyRx = false;
		};

		void setup(Printer * _printer, CommandParser * _commandParser);
		void startActivity (unsigned long activity, long stopAfter);
		
		void reset();
		
		void loop(unsigned long now);

		void lockout();

		// TODO void calibrateX ();
		// TODO void calibrateY ();
		// TODO void calibrateZ ();

	private:

		Printer * _printer;
		CommandParser * _commandParser;

		bool _readyRx;
		long _timestamp;
		long _activeAction;

};

namespace ApplicationCommand {
	const unsigned long HELP = 261238998;
	const unsigned long RESET = 2549299330;
	const unsigned long EMERGENCY_STOP = 3263447268;
	const unsigned long CALIBRATE_X = 1126058462;
	const unsigned long CALIBRATE_Y = 1126058495;
	const unsigned long CALIBRATE_Z = 1126058528;
	const unsigned long TEST_ENDSTOP_X = 1636382900;
	const unsigned long TEST_MOTORS = 779791651;
	const unsigned long NAVIGATE = 42975278;
	const unsigned long STATUS = 85226915;
	const unsigned long TEST_EXTRUDER = 1769345490;
	const unsigned long TEST_HEATBED = 851461740;
	const unsigned long LOCKOUT = 3036112128;
	const unsigned long READ_HEATBED = 3831784835;
	const unsigned long READ_EXTRUDER = 1335759817;
	const unsigned long RAISE_EXTRUDER  = 118764134;

}

#endif