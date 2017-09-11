// macOS specific libraries and functions

#ifdef __APPLE__
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
    #define glGenVertexArrays glGenVertexArraysAPPLE
    #define glBindVertexArray glBindVertexArrayAPPLE
#endif

// Math constants
const double pi = 3.14159265358979323846;

// System libraries
#include <cmath>
#include <iostream>

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

// Helper function to load vertex and fragment shader files
GLuint ShaderInit(const char* vertex_shader, const char* fragment_shader);
