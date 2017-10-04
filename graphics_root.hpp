#ifndef GRAPHICS_ROOT_HPP
#define GRAPHICS_ROOT_HPP

// macOS OpenGL
#ifdef __APPLE__
#include <OpenGL/gl.h>
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#else
#include <GL/gl.h>
#endif

// At least for Ubuntu this macro is mandatory
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#endif

// Constants
const double pi = 3.14159265358979323846;

#endif
