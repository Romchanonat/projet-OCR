# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = solver.c 
OBJ = ${SRC:.txt=.result}
DEP = ${SRC:.c=.d}

solver: ${SRC}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} solver
	${RM} a.out

# END
