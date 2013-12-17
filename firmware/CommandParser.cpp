/*	* CommandParser.cpp
	*
	* This file is subject to the terms and conditions defined in
	* file 'LICENSE', which is part of this source code package.
	*
	* Authored by: Alexander Stokes
	* License: CC by-sa 4.0
	* Owner: Alexander Stokes LLC
	* Written: 2013-12-11
	*/

#include "CommandParser.h"

CommandParser::CommandParser() {
	clear(0);
}

void CommandParser::clear (char error) {
	_hash = 5381;
	_state = 0;
	_paramAt = 0;

	_param[0] = 0;
	_param[1] = 0;
	_param[2] = 0;

	_error = error;
}

bool CommandParser::ready() {
	if (_error != 0) {
		return false;
	}
	return (_state == 2);
}

void CommandParser::read(unsigned long &hash, unsigned int &param0, unsigned int &param1, unsigned int &param2)
{
	if (_error == 0) {
		hash = _hash;
		param0 = _param[0];
		param1 = _param[1];
		param2 = _param[2];
	}
	clear(0);
}

void CommandParser::write(const char &c) {
	if (_state == 2) {
		return;
	} else if (_state == 1) {
		if (c == 44){ // ,
			_paramAt++;
		} else if (c == 41) { // )
			_state = 2;
		} else if (c >= 48 && c <= 58) {
			_param[_paramAt] = _param[_paramAt] * 10 + c - 48;
		}
	} else if (_state == 0) {
		// while receiving the command id, expects [\.A-Za-z]
		if (c == 40){ // (
			_state = 1;
		} else if ( c != 0x2E && (c <= 0x40 || c >= 0x5B) && (c <= 0x60 || c >= 0x7B) ) {
			return;
		}
		
		_hash = ((_hash << 5) + _hash) + c;
	}
	
	
	
}
