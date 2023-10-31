CPP	= gcc
CFLAGS  = `sdl-config --cflags`
LIBS	= `sdl-config --libs` -lm -lSDL_image
EXEC	= main

all: ${EXEC}

${EXEC}: ${EXEC}.o
	${CPP} ${EXEC}.c filter.c $(CFLAGS) ${LIBS} -o Filtrage_Image
# 	${CPP} $(CFLAGS) -o ${EXEC} ${EXEC}.o ${LDFLAGS}
# ${EXEC}.o: ${EXEC}.c
# 	${CPP} ${EXEC}.c traitements.c $(CFLAGS) -o ${EXEC}.o
# 	${CPP} $(CFLAGS) -o ${EXEC}.o -c ${EXEC}.c traitements.c
clean:	
	rm -fr *.o

mrproper: clean
	rm -fr ${EXEC}
