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

data_t Data_Create(dmode_t mode) {
	data_t tmp;

	tmp.mode = mode;
	tmp.length = 0;
	tmp.size = 11;
	tmp.items = calloc(sizeof(ditem_t), tmp.size);

	if (!tmp.items)
		DATA_ERR("Out of memory");

	return tmp;
}

void Data_Delete(data_t *D) {
	free(D->items);
	D->mode = EAST_DATA_CHAR;
}

static void DataDouble(data_t *D) {
	D->size = (D->size * 2) - 1;

	ditem_t *tmp = realloc(D->items, sizeof(ditem_t)*D->size);

	if (!tmp)
		DATA_ERR("Out of memory");

	D->items = tmp;
}

void Data_PushC(data_t *D, char c) {
	assert(D->mode == EAST_DATA_CHAR);

	if (D->length+1 > D->size-1)
		DataDouble(D);

	D->items[D->length].c = c;
	D->length++;
}

void Data_PushF(data_t *D, float f) {
	assert(D->mode == EAST_DATA_FLOAT);

	if (D->length+1 > D->size-1)
		DataDouble(D);

	D->items[D->length].f = f;
	D->length++;
}

void Data_PushD(data_t *D, double d) {
	assert(D->mode == EAST_DATA_DOUBLE);

	if (D->length+1 > D->size-1)
		DataDouble(D);

	D->items[D->length].d = d;
	D->length++;
}

ditem_t Data_Pop(data_t *D) {
	if (D->length == 0)
		DATA_ERR("Data empty");
	D->length--;
	ditem_t tmp = D->items[D->length];
	D->items[D->length] = (ditem_t){0};
	return tmp;
}

char Data_PopC(data_t *D) {
	return Data_Pop(D).c;
}

float Data_PopF(data_t *D) {
	return Data_Pop(D).f;
}

double Data_PopD(data_t *D) {
	return Data_Pop(D).d;
}

void Data_Rotate(data_t *D) {
	ditem_t tmp = D->items[0];

	for (size_t i = 0; i < D->length; i++)
		D->items[i] = D->items[i+1];

	D->items[D->length-1] = tmp;
}

void Data_Reverse(data_t *D) {
	ditem_t *tmp = calloc(sizeof(ditem_t), D->size);

	size_t old_i = 0;
	size_t new_i = 0;
	for (new_i = 0, old_i = D->length-1; new_i < D->length; new_i++, old_i--)
		tmp[new_i] = D->items[old_i];

	free(D->items);
	D->items = tmp;
}
