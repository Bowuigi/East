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

#include "instructions.h"

// Characters after escaping them
char escaped[128] = {
	'0','1','2','3','4','5','6','7','8',
	'9','0','1','2','3','4','5','6','7',
	'8','9','0','1','2','3','4','5','6',
	'7','8','9','0','1',' ','!','"','#',
	'$','%','&','\'','(',')','*','+',
	',','-','.','/',0,1,2,3,4,5,6,7,8,9,
	':',';','<','=','>','?','@','A','B',
	'C','D','E','F','G','H','I','J','K',
	'L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','[','\\',
	']','^','_','`','\a','\b','c','d',
	'\027','\f','g','h','i','j','k','l',
	'm','\n','o','p','q','\r','s','\t',
	'u','\v','w','x','y','z','{','|',
	'}','~','H'
};

// Input string operations (read only)

// (>) i( -- ) Go to the next character on the input string
INSTR(inst_NextChar) {
	if (E->input_index < strlen(E->input))
		E->input_index += 1;
}

// (<) i( -- ) Go to the previous character in the input string
INSTR(inst_PrevChar) {
	if (E->input_index > 0)
		E->input_index -= 1;
}

// Generic data operations

// (.) i->d( in -- char ) Push the current input character to the data
INSTR(inst_PushItem) {
	INST_PUSH_CASTED(E->input[E->input_index])
}

// (,) d( top -- ) Pop the topmost item from the data
INSTR(inst_PopItem) {
	if (E->data.length == 0)
		INST_ERR("Data empty");
	Data_Pop(&E->data);
}

// (&) d( top -- copy copy ) Duplicate the topmost item from the data
INSTR(inst_DupItem) {
	if (E->data.length == 0)
		INST_ERR("Data empty");

	// Handle modes
	ditem_t item = Data_Pop(&E->data);
	switch (E->data.mode) {
		case EAST_DATA_CHAR:
			Data_PushC(&E->data, item.c);
			Data_PushC(&E->data, item.c);
			break;
		case EAST_DATA_FLOAT:
			Data_PushF(&E->data, item.f);
			Data_PushF(&E->data, item.f);
			break;
		case EAST_DATA_DOUBLE:
			Data_PushD(&E->data, item.d);
			Data_PushD(&E->data, item.d);
			break;
	}
}

// (;) d->i( top -- char ) Pop and print the topmost character from the data
INSTR(inst_PrintChar) {
	if (E->data.length == 0)
		INST_ERR("Data empty");

	switch (E->data.mode) {
		case EAST_DATA_CHAR:
			putchar(Data_PopC(&E->data));
			break;
		case EAST_DATA_FLOAT:
			putchar(Data_PopF(&E->data));
			break;
		case EAST_DATA_DOUBLE:
			putchar(Data_PopD(&E->data));
			break;
	}
}

// (:) d->i( top -- number ) Pop and print the topmost character from the data as a number
INSTR(inst_PrintNumber) {
	if (E->data.length == 0)
		INST_ERR("Data empty");

	switch (E->data.mode) {
		case EAST_DATA_CHAR:
			printf("%i", (signed char)Data_PopC(&E->data));
			break;
		case EAST_DATA_FLOAT:
			printf("%f", Data_PopF(&E->data));
			break;
		case EAST_DATA_DOUBLE: {
				double n = Data_PopD(&E->data);

				// Print in scientific notation if it is bigger than one million, normal float like otherwise
				if (n > 1e6) {
					printf("%e", n);
				} else {
					printf("%f", n);
				}
				break;
			}
	}
}

// (+) d( item1 item2 -- result ) Add the two topmost items of the data
INSTR(inst_AddData) {
	INST_MATH_OP(b+a)
}

// (-) d( item1 item2 -- result ) Substract the two topmost items of the data
INSTR(inst_SubData) {
	INST_MATH_OP(b-a);
}

// (*) d( item1 item2 -- result ) Multiply the two topmost items of the data
INSTR(inst_MultData) {
	INST_MATH_OP(b*a);
}

// (/) d( item1 item2 -- result ) Divide the two topmost items of the data, if the top one is 0, then it gets replaced with 1 to prevent "division by zero" errors
INSTR(inst_DivData) {
	INST_MATH_OP(b / ((a != 0) ? a : 1) );
}

// (!) d( everything -> reversed ) Reverse the entire data, for example, 'a' 'b' 'c' -> 'c' 'b' 'a'
INSTR(inst_ReverseData) {
	Data_Reverse(&E->data);
}

// (@) d( bottom -> top ) Rotate the stack once (AKA put the last item first, 'a' 'b' 'c' -> 'b' 'c' 'a')
INSTR(inst_RotateData) {
	Data_Rotate(&E->data);
}

// ([a-z0-9]) e->d( char -- item ) Push the current character on the executed string
INSTR(inst_PushLiteral) {
	INST_PUSH_CASTED(E->exec[E->pc]);
}

// (\) e->d( char -- escaped_item ) Push the following character escaped, based on the hardcoded "escaped" array
INSTR(inst_PushEscaped) {
	if (E->exec[E->pc+1]) {
		INST_PUSH_CASTED(escaped[(size_t)E->exec[E->pc+1]]);
		E->pc += 1;
	} else {
		INST_ERR("Expected a character to escape, got EOF");
	}
}

// Control statements

// ([) c( -- waypoint ) Set the waypoint used in `]`, which checks the input character
INSTR(inst_SetInputWP) {
	WP_Push(&E->input_waypoint, E->pc-1);
}

// (]) c,i( waypoint,current -- ) Return (set pc) to the last input waypoint if the current character on the input string is not NUL
INSTR(inst_UseInputWP) {
	if (E->input[E->input_index]) {
		pc_t tmp = WP_Pop(&E->input_waypoint);
		E->pc = tmp;
	}
}

// ({) c( -- waypoint ) Set the waypoint used in `}`, which checks the topmost item of the data
INSTR(inst_SetDataWP) {
	WP_Push(&E->data_waypoint, E->pc-1);
}

// (}) c,d( waypoint,top -- ) Return (set pc) to the last data waypoint if the topmost item of the stack isn't NUL (or if the stack isn't empty)
INSTR(inst_UseDataWP) {
	// Check top item on the stack
	ditem_t top = E->data.items[E->data.length-2];
	switch (E->data.mode) {
		case EAST_DATA_CHAR:
			if (top.c) {
				pc_t tmp = WP_Pop(&E->data_waypoint);
				E->pc = tmp;
			}
			break;
		case EAST_DATA_FLOAT:
			if (top.f) {
				pc_t tmp = WP_Pop(&E->data_waypoint);
				E->pc = tmp;
			}
			break;
		case EAST_DATA_DOUBLE:
			if (top.d) {
				pc_t tmp = WP_Pop(&E->data_waypoint);
				E->pc = tmp;
			}
			break;
	}
}

// (?) d,c( top :2nd -- skip1 ) If the top two items on the data are equal, the next instruction is skipped, otherwise, it is executed. The last element of the data is always popped
INSTR(inst_IfNotEqual) {
	switch (E->data.mode) {
		case EAST_DATA_CHAR: {
				char a = Data_PopC(&E->data);
				char b = E->data.items[E->data.length-1].c;

				if (b == a) {
					E->pc += 1;
					if (!E->exec[E->pc]) {
						E->pc -= 1;
						INST_ERR("No instruction to skip to");
					}
				}
				break;
			}
		case EAST_DATA_FLOAT: {
				float a = Data_PopF(&E->data);
				float b = E->data.items[E->data.length-1].f;

				if (b == a) {
					E->pc += 1;
					if (!E->exec[E->pc]) {
						E->pc -= 1;
						INST_ERR("No instruction to skip to");
					}
				}
				break;
			}
		case EAST_DATA_DOUBLE: {
				float a = Data_PopF(&E->data);
				float b = E->data.items[E->data.length-1].f;

				if (b == a) {
					E->pc += 1;
					if (!E->exec[E->pc]) {
						E->pc -= 1;
						INST_ERR("No instruction to skip to");
					}
				}
				break;
			}
	}
}

// (#) e( skip -> ) Ignores everything until a newline or a NUL is found on the executed string
INSTR(inst_Comment) {
	while (E->exec[E->pc] != '\n' && E->exec[E->pc] != '\0') {
		E->pc++;
	}
}

inst_t *Inst_Get() {
	static inst_t i[127];

	// Uses executed string
	for (int c = 0; c < 127; c++)
		i[c] = inst_PushLiteral;

	i['\\'] = inst_PushEscaped;

	// Uses input string
	i['<']  = inst_PrevChar;
	i['>']  = inst_NextChar;
	// Uses on top data item
	i['.']  = inst_PushItem;
	i[',']  = inst_PopItem;
	i['&']  = inst_DupItem;
	i[';']  = inst_PrintChar;
	i[':']  = inst_PrintNumber;
	// Uses topmost two stack items
	i['+']  = inst_AddData;
	i['-']  = inst_SubData;
	i['*']  = inst_MultData;
	i['/']  = inst_DivData;
	// Uses entire stack
	i['!']  = inst_ReverseData;
	i['@']  = inst_RotateData;
	// Uses PC, controls the state of the interpreter
	i['[']  = inst_SetInputWP;
	i[']']  = inst_UseInputWP;
	i['{']  = inst_SetDataWP;
	i['}']  = inst_UseDataWP;
	i['?']  = inst_IfNotEqual;
	i['#']  = inst_Comment;

	return i;
}
