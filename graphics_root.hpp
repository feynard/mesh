#ifndef GRAPHICS_ROOT_HPP
#define GRAPHICS_ROOT_HPP

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
    #define __gl_h_
//  #define glGenVertexArrays glGenVertexArraysAPPLE
//  #define glBindVertexArray glBindVertexArrayAPPLE
//  #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#elif __linux
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
#else
    #define GL_GLEXT_PROTOTYPES
#endif

// Constants
const double pi = 3.14159265358979323846;

#endif
