/*
 * East: A stack based esolang for data manipulation
 * Copyright (C) 2021 Bowuigi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef EAST_INSTR_H
#define EAST_INSTR_H

#include "globals.h"
#define INST_ERR(err) do {fprintf(stderr, "East, error while interpreting\nCharacter %zu ('%c'): %s\n", E->pc+1, E->exec[E->pc], err); exit(1);} while (0);

// Generic macro for doing math operations, handles modes correctly
#define INST_MATH_OP(op) \
	switch (E->data.mode) { \
		case EAST_DATA_CHAR: { \
				if (E->data.length == 0) INST_ERR("Data empty"); \
				char a = Data_PopC(&E->data); \
				if (E->data.length == 0) INST_ERR("Data empty"); \
				char b = Data_PopC(&E->data); \
				Data_PushC(&E->data,op); \
				break; \
			} \
		case EAST_DATA_FLOAT: { \
				if (E->data.length == 0) INST_ERR("Data empty"); \
				float a = Data_PopF(&E->data); \
				if (E->data.length == 0) INST_ERR("Data empty"); \
				float b = Data_PopF(&E->data); \
				Data_PushF(&E->data,op); \
				break; \
			} \
		case EAST_DATA_DOUBLE: { \
				if (E->data.length == 0) INST_ERR("Data empty"); \
				float a = Data_PopD(&E->data); \
				if (E->data.length == 0) INST_ERR("Data empty"); \
				float b = Data_PopD(&E->data); \
				Data_PushD(&E->data,op); \
				break; \
			} \
	}

// Push a character to the stack, no matter which one is the mode
#define INST_PUSH_CASTED(character) \
	switch (E->data.mode) { \
		case EAST_DATA_CHAR: \
			Data_PushC(&E->data, character); \
			break; \
		case EAST_DATA_FLOAT: \
			Data_PushF(&E->data, character); \
			break; \
		case EAST_DATA_DOUBLE: \
			Data_PushD(&E->data, character); \
			break; \
	}

// Input string operations (read only)

// (>) i( -- ) Go to the next character on the input string
INSTR(inst_NextChar);

// (<) i( -- ) Go to the previous character in the input string
INSTR(inst_PrevChar);

// Generic data operations

// (.) i->d( in -- char ) Push the current input character to the data
INSTR(inst_PushItem);

// (,) d( top -- ) Pop the topmost item from the data
INSTR(inst_PopItem);

// (&) d( top -- copy copy ) Duplicate the topmost item from the data
INSTR(inst_DupItem);

// (;) d->i( top -- char ) Pop and print the topmost character from the data
INSTR(inst_PrintChar);

// (:) d->i( top -- number ) Pop and print the topmost character from the data as a number
INSTR(inst_PrintNumber);

// (+) d( item1 item2 -- result ) Add the two topmost items of the data
INSTR(inst_AddData);

// (-) d( item1 item2 -- result ) Substract the two topmost items of the data
INSTR(inst_SubData);

// (*) d( item1 item2 -- result ) Multiply the two topmost items of the data
INSTR(inst_MultData);

// (/) d( item1 item2 -- result ) Divide the two topmost items of the data, if the top one is 0, then it gets replaced with 1 to prevent "division by zero" errors
INSTR(inst_DivData);

// (!) d( everything -> reversed ) Reverse the entire data, for example, 'a' 'b' 'c' -> 'c' 'b' 'a'
INSTR(inst_ReverseData);

// (@) d( bottom -> top ) Rotate the stack once (AKA put the last item first, 'a' 'b' 'c' -> 'b' 'c' 'a')
INSTR(inst_RotateData);

// (=) d( until_NUL -- execute_result ) Read (not pop) everything until a NUL, reverse it, and execute it as East code, only being able to modify the data (the rest is isolated)
INSTR(inst_ExecData);

// ([a-z0-9]) e->d( char -- item ) Push the current character on the executed string
INSTR(inst_PushLiteral);

// (\) e->d( char -- escaped_item ) Push the following character escaped, based on the hardcoded "escaped" array
INSTR(inst_PushEscaped);

// Control statements (The ones that modify the interpreter state)

// ([) c( -- waypoint ) Set the waypoint used in `]`, which checks the input character
INSTR(inst_SetInputWP);

// (]) c,i( waypoint,current -- ) Return (set pc) to the last input waypoint if the current character on the input string is not NUL
INSTR(inst_UseInputWP);

// ({) c( -- waypoint ) Set the waypoint used in `}`, which checks the topmost item of the data
INSTR(inst_SetDataWP);

// (}) c,d( waypoint,top -- ) Return (set pc) to the last data waypoint if the topmost item of the stack isn't NUL (or if the stack isn't empty)
INSTR(inst_UseDataWP);

// (?) d,c( top :2nd -- skip1 ) If the top two items on the data are equal, the next instruction is skipped, otherwise, it is executed. The last element of the data is always popped
INSTR(inst_IfNotEqual);

// (#) e( skip -> ) Ignores everything until a newline or a NUL is found on the executed string
INSTR(inst_Comment);

inst_t *Inst_Get();

#endif // EAST_INSTR_H header guard
