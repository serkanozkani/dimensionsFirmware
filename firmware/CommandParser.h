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
		void clear(char);
		bool ready();
		void write(const char &c);
		void read(unsigned long &hash, unsigned int &param0, unsigned int &param1, unsigned int &param2);

	private:		
		unsigned long _hash;
		unsigned int _param[MAX_PARAMS];

		char _error;
		char _state;
		char _paramAt;
};

#endif