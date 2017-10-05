# Main Flags
CC = g++
GCC_FLAGS = -Wall -Werror -pedantic -std=c++11
OBJECTS = main.o mesh.o shader_init.o vec.o mat.o scene.o text_interface.o

# OS check
OS := $(shell uname)

ifeq ($(OS), Darwin)
OPENGL_FLAG = -framework OpenGL
GLUT_FRAMEWORK = -framework GLUT
GCC_FLAGS += -Wno-deprecated-declarations
else ifeq ($(OS), Linux)
OPENGL_FLAG = -lGL
GLUT_FRAMEWORK = -lglut
endif

# Targets

program: $(OBJECTS)
	$(CC) $(GCC_FLAGS) $(OBJECTS) -o program $(OPENGL_FLAG) $(GLUT_FRAMEWORK)

main.o: main.cpp graphics.hpp
	$(CC) $(GCC_FLAGS) -c main.cpp

graphics.hpp: graphics_root.hpp mesh.hpp scene.hpp

vec.o: graphics_root.hpp vec.hpp vec.cpp
	$(CC) $(GCC_FLAGS) -c vec.cpp

mat.o: graphics_root.hpp mat.hpp mat.cpp vec.hpp
	$(CC) $(GCC_FLAGS) -c mat.cpp

shader_init.o: shader_init.cpp graphics.hpp
	$(CC) $(GCC_FLAGS) -c shader_init.cpp

text_interface.o: text_interface.cpp graphics.hpp
	$(CC) $(GCC_FLAGS) -c text_interface.cpp

scene.o: scene.hpp scene.cpp list.hpp mesh.hpp graphics_root.hpp
	$(CC) $(GCC_FLAGS) -c scene.cpp

mesh.o: mesh.hpp mesh.cpp list.hpp mat.hpp vec.hpp graphics_root.hpp \
	colorscheme.hpp
	$(CC) $(GCC_FLAGS) -c mesh.cpp

clean:
	@ rm -f program -r program.dSYM *.o
