GCC=g++
LDFLAGS=-lsfml-window -lsfml-system -lGL -lGLU -lGLEW
TRIANGLE_DEPS=vbo_triangle.o
CUBE_DEPS=cube.o
ALL=triangle cube

all: $(ALL)

cube: $(CUBE_DEPS)
	$(GCC) -o $@ $(CUBE_DEPS) $(LDFLAGS)

triangle: $(TRIANGLE_DEPS)
	$(GCC) -o $@ $(TRIANGLE_DEPS) $(LDFLAGS)

%.o: %.cpp
	$(GCC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o $(ALL)
