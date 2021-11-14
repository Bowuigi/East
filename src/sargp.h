/*
 * SargP: Extremely simple argument parser heavily inspired by the one made by 20h
 * Copy me if you can
 * By Bowuigi
 * (License: Public domain, too small to have a decent license)
*/

#define ARGPARSE(arg) if (*arg == '-') { \
	arg++; \
	for (; *arg; arg++) { \
		switch (*arg)

#define ARGEND }}

#define ARGNUM case '0': \
	case '1': \
	case '2': \
	case '3': \
	case '4': \
	case '5': \
	case '6': \
	case '7': \
	case '8': \
	case '9':
