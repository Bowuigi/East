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

typedef size_t pc_t;

typedef struct {
	char *exec;
	char *input;
	pc_t pc;
	pc_t input_index;
	wp_t data_waypoint;
	wp_t input_waypoint;
	data_t data;
} East_State;

#define INSTR(name) void name(East_State *E)

typedef void(*inst_t)(East_State*);

char escaped[] = {
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
	'e','\f','g','h','i','j','k','l',
	'm','\n','o','p','q','\r','s','\t',
	'u','\v','w','x','y','z','{','|',
	'}','~','H'
};

#endif // EAST_GLOBALS_H header guard
