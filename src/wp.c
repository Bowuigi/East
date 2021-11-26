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

#include "wp.h"

wp_t WP_Create() {
	wp_t tmp;

	tmp.length = 0;
	tmp.size = 10;
	tmp.items = calloc(sizeof(size_t), tmp.size);

	if (!tmp.items)
		WP_ERR("Out of memory");

	return tmp;
}

void WP_Delete(wp_t *W) {
	free(W->items);
}

static void WPDouble(wp_t *W) {
	size_t *tmp;

	W->size *= 2;
	tmp = realloc(W->items, sizeof(size_t)*W->size);

	if (!tmp)
		WP_ERR("Out of memory");

	W->items = tmp;
}

void WP_Push(wp_t *W, size_t waypoint) {
	if (W->length+2 > W->size)
		WPDouble(W);

	W->items[W->length] = waypoint;
	W->length++;
	W->items[W->length+1] = 0;
}

size_t WP_Pop(wp_t *W) {
	if (W->length <= 0)
		return 0;
	W->length--;
	size_t tmp = W->items[W->length];
	W->items[W->length] = 0;
	return tmp;
}
