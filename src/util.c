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

#include "util.h"

// Read an entire file into a variable
char *ReadFile(size_t *length, FILE *fp) {
	size_t read_length;
	size_t file_length;

	// Get file length
	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (file_length > EAST_FILESIZE_LIMIT) {
		EAST_ERR("File too large");
	}

	// Make space for the contents and handle OOM
	char *contents = malloc(file_length+1);

	if (!contents)
		EAST_ERR("Out of memory");

	// Read the entire file and handle errors
	read_length = fread(contents, 1, file_length, fp);

	if (file_length != read_length) {
		free(contents);
		EAST_ERR("Error on file read");
	}

	// NUL terminate the string and remove ending newline
	contents[file_length] = '\0';

	if (contents[file_length-1] == '\n') {
		file_length--;
		contents[file_length] = '\0';
	}

	*length = file_length;

	return contents;
}

// Read the entirety of stdin on a single variable
char *ReadStdin(size_t *length) {
	size_t str_size = 10;
	size_t str_length = 0;
	char *contents = calloc(str_size+1, sizeof(char));

	int c = '\0';

	// Read char by char
	while (c != EOF) {
		c = getchar();

		// Handle reallocation
		if (str_length+1 > str_size) {
			str_size *= 2;
			char *tmp = realloc(contents, (str_size+1) * sizeof(char));
			if (!tmp) {
				free(contents);
				EAST_ERR("Out of memory");
			}
			contents = tmp;
		}

		// Append the character
		contents[str_length] = (char)c;
		str_length++;
	}

	// Position over EOF and NUL terminate it
	str_length--;
	contents[str_length] = '\0';

	// Remove ending newline if any
	if (contents[str_length-1] == '\n') {
		str_length--;
		contents[str_length] = '\0';
	}

	*length = str_length;
	return contents;
}
