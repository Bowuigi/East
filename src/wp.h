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

#ifndef EAST_WP_H
#define EAST_WP_H

#include <stdlib.h>
#include <assert.h>

#define WP_ERR(msg) do {fprintf(stderr,"East, fatal error: %s", msg);exit(1);} while (0)

typedef struct {
	size_t *items;
	size_t length;
	size_t size;
} wp_t;

wp_t WP_Create();
void WP_Delete(wp_t *W);
void WP_Push(wp_t *W, size_t waypoint);
size_t WP_Pop(wp_t *W);

#endif // EAST_WP_H
