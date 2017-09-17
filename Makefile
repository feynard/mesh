GCC_FLAGS = -Wall -pedantic -Wno-deprecated-declarations
GL_FRAMEWORKS = -framework OpenGL -framework GLUT

all:
	g++ $(GCC_FLAGS) $(GL_FRAMEWORKS) *.cpp -o program
clean:
	rm -r -f program.dSYM program
