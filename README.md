# East

A stack based esolang for data manipulation

## Installation

This assumes you use a UNIX-like system compatible with POSIX C99, if you aren't, then you might have to change some small parts of the code

To build it just type

```sh
make
```

And after you try it, you can install it with

```sh
sudo make install
```

Which should install it in `/usr/local/bin/`

## Usage

```
east script [file]
```

If no filename is given, East reads from standard input

The program reads every character from the first argument and then interprets each instruction

Documentation for every instruction on the language is found on [docs/DOCUMENTATION.md](docs/DOCUMENTATION.md)
