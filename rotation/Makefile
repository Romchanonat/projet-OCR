
#CC = gcc
#CPPFLAGS =
#CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lm -ldl
#LDFLAGS = -lm -ldl
#LDLIBS = `pkg-config --libs sdl2 SDL2_image`

#all: rotation

#SRC = rotation.c
#OBJ = ${SRC:.c=.o}
#EXE = ${SRC:.c=}

#rotation: rotation.o

#.PHONY: clean

#clean:
#	${RM} ${OBJ}
#	${RM} ${EXE}

# END

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2 -lSDL2_image -lm

SRC = rotation.c help.c
OBJ = $(SRC:.c=.o)
EXEC = rotate_image

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)
