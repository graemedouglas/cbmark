###############################################################################
# Author: Graeme Douglas
# A makefile for the cbmark source code.
# Notes:
# -I know the Makefile sucks.  Would love some help with this.
###############################################################################
#BUILDDIR=../bin/

CFLAGS := $(CFLAGS) -Wall -g
CC := gcc
mygcclibs := -lrt

# Main build target.
main: bin/cbmark.o
	echo "Build complete!"
bin/cbmark.o: src/cbmark.c src/cbmark.h
	$(CC) -o bin/cbmark.o -c src/cbmark.c

# Tests
tests: bin/tests/cbmark_test
	echo "Build complete!"
bin/tests/cbmark_test: src/cbmark_test.c bin/cbmark.o
ifeq ($(CC),gcc)
	$(CC) -o bin/tests/cbmark_test src/cbmark_test.c bin/cbmark.o $(mygcclibs)
else
	$(CC) -o bin/tests/cbmark_test src/cbmark_test.c bin/cbmark.o
endif

# Clean target
clean:
	rm bin/cbmark.o
clean-tests:
	rm bin/tests/cbmark_test
