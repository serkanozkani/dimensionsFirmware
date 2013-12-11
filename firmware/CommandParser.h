/*	* CommandParser.h
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-11
	*/

#ifndef CommandParser_h
#define CommandParser_h

#define MAX_PARAMS 3

#define COMMAND_PARSER_ERROR_PARAMETER_UNEXPECTED_BYTE -1
#define COMMAND_PARSER_ERROR_MANY_PARAMETERS -2
class CommandParser
{
	public:
		CommandParser();

		void clear();

		void write(const byte& c);
		
		bool ready();
		void read(unsigned long& hash, int& param1, int& param2, int& param3);

	private:
		
		unsigned long _hash;
		int _param[MAX_PARAMS];

		byte _parserState;

		bool _commandReady;


};

#endif