#ifndef GRAPHICS_ROOT_HPP
#define GRAPHICS_ROOT_HPP

// macOS OpenGL
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #define glGenVertexArrays glGenVertexArraysAPPLE
    #define glBindVertexArray glBindVertexArrayAPPLE
    #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#elif __linux
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
#else
    #define GL_GLEXT_PROTOTYPES
#endif

// Constants
const double pi = 3.14159265358979323846;

#endif
