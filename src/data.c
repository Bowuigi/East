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

#include "data.h"

// Initialize a data_t with the given mode
data_t Data_Create(dmode_t mode) {
	data_t tmp;

	tmp.mode = mode;
	tmp.length = 0;
	tmp.size = 10;
	tmp.items = calloc(sizeof(ditem_t), tmp.size);

	// Handle OOM after allocation
	if (!tmp.items)
		DATA_ERR("Out of memory");

	return tmp;
}

// Destroy a data_t after it has fulfilled its purpose
void Data_Delete(data_t *D) {
	free(D->items);
	D->mode = EAST_DATA_CHAR;
}

// Function only used on this file that doubles the size of the data_t structure, used for pushing
static void DataDouble(data_t *D) {
	D->size *= 2;

	// Allocate and handle OOM
	ditem_t *tmp = realloc(D->items, sizeof(ditem_t)*D->size);

	if (!tmp)
		DATA_ERR("Out of memory");

	D->items = tmp;
}

// Push a char to the data_t structure, doubling the size if necessary
void Data_PushC(data_t *D, char c) {
	// This should always be true, but just in case
	assert(D->mode == EAST_DATA_CHAR);

	// Check if doubling is required
	if (D->length+1 > D->size)
		DataDouble(D);

	// Actually push the character
	D->items[D->length].c = c;
	D->length++;
}

// The exact same as the above function, but push a float instead
void Data_PushF(data_t *D, float f) {
	assert(D->mode == EAST_DATA_FLOAT);

	if (D->length+1 > D->size)
		DataDouble(D);

	D->items[D->length].f = f;
	D->length++;
}

// The exact same as the push char function, but push a double instead
void Data_PushD(data_t *D, double d) {
	assert(D->mode == EAST_DATA_DOUBLE);

	if (D->length+1 > D->size)
		DataDouble(D);

	D->items[D->length].d = d;
	D->length++;
}

// Pop a raw ditem_t, used in the functions below
ditem_t Data_Pop(data_t *D) {
	if (D->length == 0)
		DATA_ERR("Data empty");
	D->length--;
	ditem_t tmp = D->items[D->length];
	D->items[D->length] = (ditem_t){0};
	return tmp;
}

// Pop a character from the data_t structure
char Data_PopC(data_t *D) {
	return Data_Pop(D).c;
}

// Pop a float from the data_t structure
float Data_PopF(data_t *D) {
	return Data_Pop(D).f;
}

// Pop a double from the data_t structure
double Data_PopD(data_t *D) {
	return Data_Pop(D).d;
}

// Rotate (123 -> 231) the items on the data_t structure
void Data_Rotate(data_t *D) {
	// First item on the data
	ditem_t tmp = D->items[0];

	// Move every element one slot backwards
	for (size_t i = 0; i < D->length; i++)
		D->items[i] = D->items[i+1];

	// Set what used to be the first item to be the last one
	D->items[D->length-1] = tmp;
}

// Reverse the items on the data_t data structure
void Data_Reverse(data_t *D) {
	// Create another items container to hold the reversed items
	ditem_t *tmp = calloc(sizeof(ditem_t), D->size);

	// Use a single for loop to reverse the entire items array
	size_t old_i = 0;
	size_t new_i = 0;
	for (new_i = 0, old_i = D->length-1; new_i < D->length; new_i++, old_i--)
		tmp[new_i] = D->items[old_i];

	// Delete the old, not reversed array
	free(D->items);
	// Replace it with the new, reversed array
	D->items = tmp;
}
