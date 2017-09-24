GCC_FLAGS = -Wall -pedantic-errors -Wno-deprecated-declarations
OPENGL_FLAG = -framework OpenGL
GLUT_FRAMEWORK = -framework GLUT
OBJECTS =  main.o scene.o mesh.o mat.o vec.o shader_init.o

program: $(OBJECTS)
	g++ $(GCC_FLAGS) $(OPENGL_FLAG) $(GLUT_FRAMEWORK) $(OBJECTS) -o program

main.o: main.cpp graphics.hpp
	g++ $(GCC_FLAGS) -c main.cpp

shader_init.o: shader_init.cpp graphics.hpp
	g++ $(GCC_FLAGS) -c shader_init.cpp

mat.o: mat.hpp mat.cpp vec.o
	g++ $(GCC_FLAGS) -c mat.cpp

vec.o: vec.hpp vec.cpp
	g++ $(GCC_FLAGS) -c vec.cpp

scene.o: scene.hpp scene.cpp list.hpp mesh.o
	g++ $(GCC_FLAGS) -c scene.cpp

mesh.o: mesh.hpp mesh.cpp list.hpp
	g++ $(GCC_FLAGS) -c mesh.cpp

clean:
	rm -f *.o program
