#ifndef EAST_UTIL_H
#define EAST_UTIL_H

char *ReadFile(size_t *length, FILE *fp) {
	size_t read_length;
	size_t file_length;

	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (file_length > EAST_FILESIZE_LIMIT) {
		EAST_ERR("File too large");
	}

	char *contents = malloc(file_length+1);

	if (!contents)
		EAST_ERR("Out of memory");

	read_length = fread(contents, 1, file_length, fp);

	if (file_length != read_length) {
		free(contents);
		EAST_ERR("Error on file read");
	}

	contents[file_length] = '\0';

	if (contents[file_length-1] == '\n') {
		file_length--;
		contents[file_length] = '\0';
	}

	*length = file_length;

	return contents;
}

char *ReadStdin(size_t *length) {
	size_t str_size = 10;
	size_t str_length = 0;
	char *contents = calloc(str_size+1, sizeof(char));

	int c = '\0';

	while (c != EOF) {
		c = getchar();
		
		if (str_length+1 > str_size) {
			str_size *= 2;
			char *tmp = realloc(contents, (str_size+1) * sizeof(char));
			if (!tmp) {
				free(contents);
				EAST_ERR("Out of memory");
			}
			contents = tmp;
		}

		contents[str_length] = (char)c;
		str_length++;
	}

	str_length--; // Position over EOF
	contents[str_length] = '\0';

	if (contents[str_length-1] == '\n') {
		str_length--;
		contents[str_length] = '\0';
	}

	return contents;
}

#endif
