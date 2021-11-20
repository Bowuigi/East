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

#ifndef EAST_GLOBALS_H
#define EAST_GLOBALS_H

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structures
#include "data.h"
#include "wp.h"

// Define the type used by the program counter and by the waypoints
typedef size_t pc_t;

struct East_State;

// Function pointer for instruction array
typedef void(*inst_t)(struct East_State*);

// State which holds all the relevant variables for executing East code
typedef struct East_State {
	char *exec;
	char *input;
	pc_t pc;
	pc_t input_index;
	wp_t data_waypoint;
	wp_t input_waypoint;
	data_t data;
	inst_t *instr;
} East_State;

// Macro to easily define instructions
#define INSTR(name) void name(East_State *E)

void ExecuteString(char *string, data_t *data, inst_t *instr, char *input);

#endif // EAST_GLOBALS_H
