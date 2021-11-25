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

// Useful macros when arg parsing
#define VERSION puts("East 2.2.0")
#define USAGE puts("East - Stack based esolang for text processing\n\n\
east [flags] script [file]\n\n\
If file is not specified and -n is not used, read from standard input\n\n\
One argument mode only:\n\
 -W Show warranty\n\
 -C Show copyright\n\
 -V Show version (East x.x.x)\n\
 -h Show help\n\
\n\
Two or three argument mode only:\n\
 -c Use char mode (the default)\n\
 -f Use float mode\n\
 -d Use double mode\n\
 -n Don't use an input file or read standard input\n\
 -F Read script from the file instead of from the argument directly")

#define WARRANTY puts("This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.")

#define COPYRIGHT puts("East: A stack based esolang for data manipulation\n\
Copyright (C) 2021 Bowuigi\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.")

#define EAST_ERR(msg) do {fprintf(stderr,"East: %s\n", msg); exit(1);} while(0)
#define EAST_FILESIZE_LIMIT 1073741824 // 1 GiB

#include "instructions.h"
#include "util.h"
#include "sargp.h"

// Execute a string on an isolated container, only provides access to the data and the input string
void ExecuteString(char *string, data_t *data, inst_t *instr, uinst_t **userinstr, char *input) {
	East_State E;
	// Program counter
	E.pc = 0;
	// Current character on the input string
	E.input_index = 0;
	// Waypoints for both of the previous variables, used to go back
	E.data_waypoint  = WP_Create();
	E.input_waypoint = WP_Create();

	E.exec  = string;
	E.input = input;
	E.data  = *data;
	E.instr = instr;
	E.userinstr = *userinstr;

	// Execute the instruction given in the table
	for (E.pc = 0; E.pc < strlen(string); E.pc++) {
		// Execute instruction if the current character is not a newline or an underscore, since they are used for readability
		if (!(string[E.pc] == '\n' || string[E.pc] == '_' || string[E.pc] == '\t'))
			// Cast to int because characters can't be array sunscripts, but literal characters can
			instr[(int)string[E.pc]](&E);
	}

	// Cleanup
	WP_Delete(&E.data_waypoint);
	WP_Delete(&E.input_waypoint);
	*data = E.data;
}

int main(int argc, char **argv) {
	// Default initialization
	size_t input_length = 0;
	char *input;
	dmode_t mode = EAST_DATA_CHAR;
	int use_input = 1;
	int use_script_file = 0;

	// Argument parsing starts here
	switch (argc-1) {
		// Usage on zero args
		case 0:
			USAGE;
			return 1;
			break;
		// Check flags or script on one arg
		case 1: {
			// Exit if it is one of the following
			ARGPARSE(argv[1]) {
				case 'h':
					USAGE;
					break;
				case 'V':
					VERSION;
					break;
				case 'W':
					WARRANTY;
					break;
				case 'C':
					COPYRIGHT;
					break;
				default:
					fprintf(stderr,"East, warning: Unknown argument '-%c'\n", *argv[1]);
					break;
			}}return 0;}

			// Otherwise, load normally and get input from stdin
			input = ReadStdin(&input_length);
			inst_t *instructions = Inst_Get();
			uinst_t *user_instructions = Inst_UCreate();
			data_t data = Data_Create(mode);

			ExecuteString(argv[1], &data, instructions, &user_instructions, input);
			break;
		}
		// Check if it is 'flags, script' or 'script, file'. Act accordingly
		case 2: {
			// If at least one of those is chosen, the second arg is a script
			ARGPARSE(argv[1]) {
				case 'c':
					mode = EAST_DATA_CHAR;
					break;
				case 'f':
					mode = EAST_DATA_FLOAT;
					break;
				case 'd':
					mode = EAST_DATA_DOUBLE;
					break;
				case 'n':
					use_input = 0;
					break;
				case 'F':
					use_script_file = 1;
					break;
				default:
					fprintf(stderr,"East, warning: Unknown argument '-%c'\n", *argv[1]);
					break;
			}}
				// The usual preparation for execution
				inst_t *instructions = Inst_Get();
				uinst_t *user_instructions = Inst_UCreate();
				data_t data = Data_Create(mode);

				if (use_input) {
					// The '_' is a unused variable
					size_t _;
					input = ReadStdin(&_);
				} else {
					// This is so you can use square brackets to do loops
					input = "0";
				}

				// Read the East code from a file (whose filename was supplied on the given argument)
				if (use_script_file) {
					FILE *fp = fopen(argv[2], "r");

					if (fp == NULL)
						EAST_ERR("No such file");

					size_t unused;

					// Execute normally
					ExecuteString(ReadFile(&unused, fp), &data, instructions, &user_instructions, input);

					fclose(fp);
				} else {
					// Execute as in older versions
					ExecuteString(argv[2], &data, instructions, &user_instructions, input);
				}

				// Usual cleanup
				Data_Delete(&data);
			} else {
				// This is how East was executed before command line parsing
				inst_t *instructions = Inst_Get();
				uinst_t *user_instructions = Inst_UCreate();
				data_t data = Data_Create(mode);

				FILE *fp = fopen(argv[2], "r");

				if (fp == NULL)
					EAST_ERR("No such file");

				// Input comes from the given file
				input = ReadFile(&input_length, fp);

				fclose(fp);

				// Code comes from the first argument and gets executed
				ExecuteString(argv[1], &data, instructions, &user_instructions, input);

				Data_Delete(&data);
			}
			break;
		}
		case 3: {
			// Same argument parsing as before
			ARGPARSE(argv[1]) {
				case 'c':
					mode = EAST_DATA_CHAR;
					break;
				case 'f':
					mode = EAST_DATA_FLOAT;
					break;
				case 'd':
					mode = EAST_DATA_DOUBLE;
					break;
				case 'n':
					use_input = 0;
					break;
				case 'F':
					use_script_file = 1;
					break;
				default:
					fprintf(stderr,"East, warning: Unknown argument '-%c'\n", *argv[1]);
					break;
			} ARGEND

			// Same preparation
			inst_t *instructions = Inst_Get();
			uinst_t *user_instructions = Inst_UCreate();
			data_t data = Data_Create(mode);

			// Same check for -n flag
			if (use_input) {
				FILE *fp = fopen(argv[3], "r");

				if (fp == NULL)
					EAST_ERR("No such file");

				input = ReadFile(&input_length, fp);

				fclose(fp);
			} else {
				// Used like this because of []
				input = "0";
			}

			// Same check for script file
			if (use_script_file) {
				FILE *fp = fopen(argv[2], "r");

				if (fp == NULL)
					EAST_ERR("No such file");

				size_t size;

				ExecuteString(ReadFile(&size, fp), &data, instructions, &user_instructions, input);

				fclose(fp);
			} else {
				ExecuteString(argv[2], &data, instructions, &user_instructions, input);
			}

			// Same cleanup
			Data_Delete(&data);
			break;
		}
		// Show usage to help newcomers
		default:
			USAGE;
			break;
	}

	// This is for pretty output and also to flush stdout
	putchar('\n');
}
