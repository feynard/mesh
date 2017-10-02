#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "graphics_root.hpp"
#include "colorscheme.hpp"
#include "scene.hpp"
#include "mesh.hpp"

// Helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

// Window manager
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

// Text cheat shit Draw
void DrawInterface(GLuint color_location);

// Helper function to load vertex and fragment shader files
GLuint ShaderInit(const char* vertex_shader, const char* fragment_shader);

#endif
