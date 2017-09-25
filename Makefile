GCC_FLAGS = -Wall -pedantic -Wno-deprecated-declarations
OPENGL_FLAG = -framework OpenGL
GLUT_FRAMEWORK = -framework GLUT
OBJECTS = main.o scene.o mesh.o shader_init.o vec.o mat.o

program: $(OBJECTS)
	g++ $(GCC_FLAGS) $(OPENGL_FLAG) $(GLUT_FRAMEWORK) $(OBJECTS) -o program

main.o: main.cpp graphics.hpp mesh.hpp scene.hpp
	g++ $(GCC_FLAGS) -c main.cpp

graphics.hpp: vec.hpp mat.hpp

vec.o: vec.hpp vec.cpp
	g++ $(GCC_FLAGS) -c vec.cpp

mat.o: mat.hpp mat.cpp vec.hpp
	g++ $(GCC_FLAGS) -c mat.cpp

shader_init.o: shader_init.cpp graphics.hpp
	g++ $(GCC_FLAGS) -c shader_init.cpp

scene.o: scene.hpp scene.cpp list.hpp mesh.hpp graphics.hpp
	g++ $(GCC_FLAGS) -c scene.cpp

mesh.o: mesh.hpp mesh.cpp list.hpp graphics.hpp
	g++ $(GCC_FLAGS) -c mesh.cpp

clean:
	@ rm -f program -r program.dSYM *.o
