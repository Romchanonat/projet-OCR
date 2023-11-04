CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: solver

solver: solver.c box.c puzzle.c row.c square.c solver.h
	$(CC) $(CFLAGS) -o solver solver.c box.c puzzle.c row.c square.c

clean:
	rm -f solver
