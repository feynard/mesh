// macOS specific libraries and functions

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#ifdef __APPLE__
    #define glGenVertexArrays glGenVertexArraysAPPLE
    #define glBindVertexArray glBindVertexArrayAPPLE
    #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

// Math constants
const double pi = 3.14159265358979323846;

// Algebra
#include "vec.hpp"
#include "mat.hpp"

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

// Helper function to load vertex and fragment shader files
GLuint ShaderInit(const char* vertex_shader, const char* fragment_shader);

#endif
