CC = cc
OPT = -Os
CFLAGS = -Wall -Wpedantic -std=c99
DEBUGCFLAGS =-O0 -ggdb -Wall -Wpedantic -std=c99
MKDIRP = mkdir -p
DESTDIR = /usr/local/bin/

build:
	@echo 'Building...'
	$(CC) $(OPT) $(CFLAGS) src/east.c -o east

debug:
	@echo 'Building a debug release...'
	$(CC) $(DEBUGCFLAGS) src/east.c -o east

install: build
	@echo 'Installing...'
	$(MKDIRP) $(DESTDIR)
	cp east $(DESTDIR)

uninstall:
	@echo 'Uninstalling...'
	rm $(DESTDIR)/east
