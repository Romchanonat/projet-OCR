# Makefile

CPPFLAGS = -MMD 
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic `pkg-config --cflags sdl2 SDL2_image` -O2 -g3 #-fsanitize=address
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = main.c HoughTransform.c Pixel.c CutGrid.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

.PHONY: clean
clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program
	${RM} Pieces/*.bmp #remove all image in /Pieces/
	${RM} *.bmp


-include ${DEP}

# END
