#include "graphics_support.hpp"
#include "mat.hpp"
#include "scene.hpp"

using namespace std;

// Rotation parameter
GLuint Theta;
GLfloat theta[2];

GLuint CamPos, CamRot;

GLuint Color;
vec4 color(220 / 255.0, 50 / 255.0, 47 / 255.0, 1.0);

Mesh my_mesh_1, my_mesh_2;
Scene my_scene;                 // Main scene
GLuint vao;                     // Main vertex array

void Init(int argc, char **argv)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Load shaders and use the resulting shader program
    GLuint program = ShaderInit("shader.vert", "shader.frag");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation(program, "position");
    Theta = glGetUniformLocation(program, "theta");
    Color = glGetUniformLocation(program, "edge_color");
    CamPos = glGetUniformLocation(program, "cam_position");
    CamRot = glGetUniformLocation(program, "cam_rotation");

    my_scene.init(Color, CamPos, CamRot);

    if (argc > 1)
        my_mesh_1.load(argv[1]);
    else
        my_mesh_1.load("obj_files/teapot.obj");

    my_mesh_1.color(Color);
    my_scene.add_object(&my_mesh_1);
    my_scene.add_camera(vec3(0.2, 0.2, 0.2), vec3(0.61548, 3 * pi / 4, 0));

    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnable(GL_DEPTH_TEST);

    glClearColor(0 / 255.0, 43 / 255.0, 54 / 255.0, 1.0);
}


// Default vieport size
GLint Width = 600, Height = 600;
GLsizei CurrentWidth = 960, CurrentHeight = 600;

// Interface commands list
const char *interface[] = {
    "Quit (ESC)",
    "Rotation (r)",
    "Vertex Normals (v)",
    "Faces Normals (f)",
    "Bounding Box (b)"
};

// Draw list of commands
void DrawInterface(GLuint color_location)
{
    glUniform4f(color_location, 42 / 255.0, 161 / 255.0, 152 / 255.0, 1);

    int x = 5, y = 5;

    for (int i = 0; i < sizeof(interface) / sizeof(char*); i++) {
        glWindowPos2i(x, y + i * 18);
        for (int j = 0; j < strlen(interface[i]); j++)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, interface[i][j]);
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    my_scene.draw();

    DrawInterface(Color);

    glutSwapBuffers();
}


void reshape(GLsizei w, GLsizei h)
{
    CurrentWidth = w, CurrentHeight = h;
    glViewport((w - Width) / 2, (h - Height) / 2, Width, Height);
}


void keyboard(int key, int x, int y)
{
    if (key == 033)
        exit(EXIT_SUCCESS);

    if (key == 'v') {
        my_mesh_1.toogle_vertex_normals();
        glutPostRedisplay();
    }
    if (key == 'f') {
        my_mesh_1.toogle_face_normals();
        glutPostRedisplay();
    }
    if (key == 'b') {
        my_mesh_1.toogle_bounding_box();
        glutPostRedisplay();
    }
    if (key == 'r') {
        theta[0] += 0.01;

        if (theta[0] >= 2 * pi)
            theta[0] -= 2 * pi;

        glutPostRedisplay();
    }

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(960, 600);
    glutCreateWindow("Mesh");

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutReshapeFunc(reshape);

    Init(argc, argv);

    glutMainLoop();

    return 0;
}
