# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = main.c 
OBJ = ${SRC:.txt=.result}
DEP = ${SRC:.c=.d}

solver: ${SRC}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${DEP}
	${RM} a.out

# END
