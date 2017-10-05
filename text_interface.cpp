#include "graphics.hpp"
#include <cstring>

// Interface commands list
const char * interface[] = {
    "Quit (ESC)",
    "Vertex Normals ('v')",
    "Faces Normals ('f')",
    "Bounding Box ('b')",
    "Switch Between Cameras ('['', ']')",
    "Add Camera ('c')",
    "Camera Rotation (Alt + LMB)",
    "Camera Move (Alt + MMB)",
    "Camera Zoom (Alt + RMB)",
    "Camera Roll (Ctrl + LMB)",
    "Delete Camera ('d')",
    "Switch Between Objects ('9', '0')",
    "Move ('w')",
    "Scale ('r')",
    "Rotate ('e')"
};

// Draw list of commands
void DrawInterface(GLuint color_location)
{
    glUniform4f(color_location, 42 / 255.0, 161 / 255.0, 152 / 255.0, 1);

    int x = 5, y = 5;

    for (unsigned int i = 0; i < sizeof(interface) / sizeof(char*); i++) {
        glWindowPos2i(x, y + i * 18);
        for (unsigned int j = 0; j < strlen(interface[i]); j++)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, interface[i][j]);
    }
}
