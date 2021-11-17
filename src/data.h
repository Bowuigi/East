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

#ifndef EAST_DATA_H
#define EAST_DATA_H

#include <stdlib.h>
#include <assert.h>

#define DATA_ERR(msg) do {fprintf(stderr,"East, fatal error: %s\n", msg);exit(1);} while (0)

typedef enum {
	EAST_DATA_FLOAT,
	EAST_DATA_DOUBLE,
	EAST_DATA_CHAR
} dmode_t;

typedef union {
	double d;
	float f;
	char c;
} ditem_t;

typedef struct {
	dmode_t mode;
	ditem_t *items;
	size_t length;
	size_t size;
} data_t;

data_t Data_Create(dmode_t mode);
void Data_Delete(data_t *D);
void Data_PushC(data_t *D, char c);
void Data_PushF(data_t *D, float f);
void Data_PushD(data_t *D, double d);
ditem_t Data_Pop(data_t *D);
char Data_PopC(data_t *D);
float Data_PopF(data_t *D);
double Data_PopD(data_t *D);
void Data_Rotate(data_t *D);
void Data_Reverse(data_t *D);

#endif // EAST_DATA_H
