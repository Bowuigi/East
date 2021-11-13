#ifndef EAST_INSTR_H
#define EAST_INSTR_H

#include "globals.h"
#define INST_ERR(err) do {fprintf(stderr, "East, error while interpreting\nCharacter %zu ('%c'): %s\n", E->pc+1, E->exec[E->pc], err); exit(1);} while (0);
//extern void ExecuteString(char*, data_t*, inst_t*, char*);

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

#define INST_PUSH_CASTED(character) \
	switch (E->data.mode) { \
		case EAST_DATA_CHAR: \
			Data_PushC(&E->data, character); \
			break; \
		case EAST_DATA_FLOAT: \
			Data_PushF(&E->data, (float)character); \
			break; \
		case EAST_DATA_DOUBLE: \
			Data_PushD(&E->data, (double)character); \
			break; \
	}

//# Input string operations (read only)

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

//# Generic data operations

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
	if (E->data.length > 0) {
		pc_t tmp = WP_Pop(&E->data_waypoint);
		E->pc = tmp;
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
	i['}']  = inst_UseDataWP;
	i['?']  = inst_IfNotEqual;
	i['#']  = inst_Comment;

	return i;
}

#endif // EAST_INSTR_H header guard
