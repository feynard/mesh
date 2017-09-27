CC = g++
GCC_FLAGS = -Wall -pedantic -Wno-deprecated-declarations
OPENGL_FLAG = -framework OpenGL
GLUT_FRAMEWORK = -framework GLUT
OBJECTS = main.o scene.o mesh.o shader_init.o vec.o mat.o

program: $(OBJECTS)
	$(CC) $(GCC_FLAGS) $(OPENGL_FLAG) $(GLUT_FRAMEWORK) $(OBJECTS) -o program

main.o: main.cpp graphics.hpp
	$(CC) $(GCC_FLAGS) -c main.cpp

graphics.hpp: graphics_root.hpp mesh.hpp scene.hpp

vec.o: graphics_root.hpp vec.hpp vec.cpp
	$(CC) $(GCC_FLAGS) -c vec.cpp

mat.o: graphics_root.hpp mat.hpp mat.cpp vec.hpp
	$(CC) $(GCC_FLAGS) -c mat.cpp

shader_init.o: shader_init.cpp graphics.hpp
	$(CC) $(GCC_FLAGS) -c shader_init.cpp

scene.o: scene.hpp scene.cpp list.hpp mesh.hpp graphics_root.hpp
	$(CC) $(GCC_FLAGS) -c scene.cpp

mesh.o: mesh.hpp mesh.cpp list.hpp graphics_root.hpp
	$(CC) $(GCC_FLAGS) -c mesh.cpp

clean:
	@ rm -f program -r program.dSYM *.o
