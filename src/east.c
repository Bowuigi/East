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

#define VERSION puts("East 2.0.3")
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
	size_t input_length = 0;
	char *input;
	dmode_t mode = EAST_DATA_CHAR;
	int use_input = 1;
	int use_script_file = 0;

	switch (argc-1) {
		case 0:
			USAGE;
			return 1;
			break;
		case 1: {
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
			}}return 0;}

			input = ReadStdin(&input_length);
			inst_t *instructions = Inst_Get();
			data_t data = Data_Create(mode);

			ExecuteString(argv[1], &data, instructions, input);
			break;
		}
		case 2: {
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
			}}
				inst_t *instructions = Inst_Get();
				data_t data = Data_Create(mode);

				if (use_input) {
					size_t _;
					input = ReadStdin(&_);
				} else {
					input = "0";
				}

				if (use_script_file) {
					FILE *fp = fopen(argv[2], "r");

					if (fp == NULL)
						EAST_ERR("No such file");

					size_t unused;

					ExecuteString(ReadFile(&unused, fp), &data, instructions, input);

					fclose(fp);
				} else {
					ExecuteString(argv[2], &data, instructions, input);
				}

				Data_Delete(&data);
			} else {
				inst_t *instructions = Inst_Get();
				data_t data = Data_Create(mode);

				FILE *fp = fopen(argv[2], "r");

				if (fp == NULL)
					EAST_ERR("No such file");

				input = ReadFile(&input_length, fp);

				fclose(fp);

				ExecuteString(argv[1], &data, instructions, input);

				Data_Delete(&data);
			}
			break;
		}
		case 3: {
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
			} ARGEND

			inst_t *instructions = Inst_Get();
			data_t data = Data_Create(mode);

			if (use_input) {
				FILE *fp = fopen(argv[3], "r");

				if (fp == NULL)
					EAST_ERR("No such file");

				input = ReadFile(&input_length, fp);

				fclose(fp);
			} else {
				input = "0";
			}

			if (use_script_file) {
				FILE *fp = fopen(argv[2], "r");

				if (fp == NULL)
					EAST_ERR("No such file");

				fclose(fp);

				size_t _;

				ExecuteString(ReadFile(&_, fp), &data, instructions, input);
			} else {
				ExecuteString(argv[2], &data, instructions, input);
			}

			Data_Delete(&data);
			break;
		}
		default:
			USAGE;
			break;
	}

	putchar('\n');
}
