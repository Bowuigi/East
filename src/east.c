#include "instructions.h"
#define USAGE puts("East - Stack based esolang for text processing\n\neast script [file]")
#define EAST_ERR(cond, msg) if (cond) {fprintf(stderr, msg); exit(1);}

char *ReadFile(size_t *length, FILE *fp) {
	char *contents = malloc(sizeof(char));
	size_t tmp;

	if (getdelim(&contents, &tmp, '\0', fp) == -1)
		contents[0] = '\0';

	*length = strlen(contents);

	if (contents[*length-1] == '\n')
		contents[*length-1] = '\0';

	return contents;
}

void ExecuteString(char *string, data_t *data, inst_t *instr, char *input) {
	East_State E;
	// Program counter
	E.pc = 0;
	// Current character on the input string
	E.input_index = 0;
	// Waypoints for both of the previous variables, used to go back
	E.data_waypoint  = WP_Create();
	E.input_waypoint = WP_Create();

	// Instruction given
	E.exec  = string;
	E.input = input;
	E.data  = *data;

	// Execute the instruction given in the table
	for (E.pc = 0; E.pc < strlen(string); E.pc++) {
		// Execute instruction if the current character is not a newline or an underscore, since they are used for readability
		if (!(string[E.pc] == '\n' || string[E.pc] == '_'))
			instr[(int)string[E.pc]](&E);
	}

	// Cleanup
	WP_Delete(&E.data_waypoint);
	WP_Delete(&E.input_waypoint);
	*data = E.data;
}

int main(int argc, char **argv) {
	if (argc != 2 && argc != 3) {
		USAGE;
		return 1;
	}

	size_t input_length = 0;
	char *input;

	if (argc == 3) {
		FILE *fp = fopen(argv[2], "r");
		EAST_ERR(fp == NULL, "East: no such file\n");

		input = ReadFile(&input_length, fp);

		fclose(fp);
	} else {
		input = ReadFile(&input_length, stdin);
	}

	inst_t *instructions = Inst_Get();
	data_t data = Data_Create(EAST_DATA_CHAR);

	ExecuteString(argv[1], &data, instructions, input);

	Data_Delete(&data);
	putchar('\n');
}
