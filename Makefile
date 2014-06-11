GCC=g++
CFLAGS=-lsfml-window -lsfml-system -lGL -lGLU

triangle: triangle.o
	${GCC} -o $@ triangle.o ${CFLAGS}
