#include "graphics_support.hpp"
#include "mat.hpp"
#include "mesh.hpp"

using namespace std;

// Rotation parameter
GLuint Theta;
double theta = 0;

// Main geometric container
int number_of_points;
vec3 *geometry;

void init(int argc, char **argv)
{
    Mesh my_mesh("obj_files/blob.obj");

    geometry = my_mesh.faces();
    number_of_points = my_mesh.faces_number() * 3;

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, number_of_points * sizeof(vec3), geometry,
        GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    GLuint program = ShaderInit("shader.vert", "shader.frag");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    Theta = glGetUniformLocation(program, "theta");

    glClearColor(7.0 / 255, 54.0 / 255, 66.0 / 255, 1.0);
}


// Default vieport size
GLint Width = 600, Height = 600;
GLsizei CurrentWidth = 960, CurrentHeight = 600;
char str[] = "Quit";

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(Theta, theta);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, number_of_points);

    glWindowPos2i(5, 5);
    for (int i = 0; i < strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);

    glFlush();
}

void reshape(GLsizei w, GLsizei h)
{
    CurrentWidth = w, CurrentHeight = h;
    glViewport((w - Width) / 2, (h - Height) / 2, Width, Height);
}

void mouse(int button, int state, int x, int y)
{
    // Quit button
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        if (5 <= x && x <= 37 &&
            CurrentHeight - 5 >= y && y >= CurrentHeight - 18)
            exit(EXIT_SUCCESS);
}

void idle() {
    theta += 0.01;

    if (theta >= 2 * pi)
        theta -= 2 * pi;

    glutPostRedisplay();
}

bool play = true;
void keyboard(int key, int x, int y)
{
    if (key == 033)
        exit(EXIT_SUCCESS);
    if (key == ' ') {
        play ? glutIdleFunc(NULL) : glutIdleFunc(idle);
        play ? play = false : play = true;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(960, 600);
    glutCreateWindow("Mesh");

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    init(argc, argv);

    glutMainLoop();

    return 0;
}
