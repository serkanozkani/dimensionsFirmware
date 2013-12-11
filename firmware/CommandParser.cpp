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
	_param[0] = 0;
	_param[1] = 0;
	_param[2] = 0;

	_commandReady = false;
	_parserState = 0;
	_error = error;
}

bool CommandParser::ready() {
	if (_error != 0) {
		return false;
	}
	return _commandReady;
}

void CommandParser::read(unsigned long& hash, int& param1, int& param2, int& param3)
{
	if (_error == 0) {
		hash = _hash;
		param1 = _param[1];
		param2 = _param[2];
		param3 = _param[3];
	}
	clear(0);
}


void CommandParser::write(const byte& c) {
	Serial.println ("C: "+c);

	switch (_parserState) {
		case (0):
			// while receiving the command id, expects [\.A-Za-z]
			if ( c != 0x2E && (c <= 0x40 || c >= 0x5B) && (c <= 0x60 || c >= 0x7B) )
				_parserState++;
			break;

		default:
			// while receiving a parameter, a comma advances the parameter id.
			if ( c == ','))
				_parserState++;
			if ( c == ')') {
				_commandReady = true;
				return;
			}
			if ( c == '(') {
				// ignore this character.
				return;
			}
			if (c > 0x3A || c < 0x30) {
				Serial.println("Error with input. Parameter field expects [0-9]*.");
				clear(COMMAND_PARSER_ERROR_PARAMETER_UNEXPECTED_BYTE);
			}
			break;
	}
	Serial.println ("_parserState: "+ _parserState);
	
	switch (_parserState) {
		case (0):
			hash = ((hash << 5) + hash) + c;
			// put this character into the hash.
			break;

		case (MAX_PARAMS):
			// The user sent too many parameters in the command.
			Serial.println("Error with input. Too many parameters.");
			clear(COMMAND_PARSER_ERROR_MANY_PARAMETERS);
			break;
		case default:
			_param[_parserState-1] = _param[_parserState-1] *10 + c & 0xF;
			break;
	}
	
}
