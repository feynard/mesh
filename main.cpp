#include "graphics.hpp"

using namespace std;

// Shader attributes:
// Camera - camera transformations, array of two 3-vectors [pos, rot]
// Local - same as camera, that is [local_pos, local_rot]
// Colour - main fragment colour attribute
GLuint Camera, Local, Colour;

// Main graphics and geometry handler
Scene my_scene;

void Init(int argc, char **argv)
{
    // Load shaders and use the resulting shader program
    GLuint program = ShaderInit("shader.vert", "shader.frag");
    glUseProgram(program);

    // Set shader attributes
    GLuint loc = glGetAttribLocation(program, "vertex_position");

    Camera = glGetUniformLocation(program, "cam");
    Local  = glGetUniformLocation(program, "loc");
    Colour = glGetUniformLocation(program, "colour");

    my_scene.init(Colour, Camera, Local);

    my_scene.add_direct("obj_files/cube.obj", solarized);
    my_scene.add_direct("obj_files/ico.obj", solarized);

    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnable(GL_DEPTH_TEST);

    glClearColor(0 / 255.0, 43 / 255.0, 54 / 255.0, 1.0);
}


// Default vieport size
const GLint Width = 600, Height = 600;
GLsizei CurrentWidth = 960, CurrentHeight = 600;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    my_scene.draw();
    DrawInterface(Colour);
    glutSwapBuffers();
}


void reshape(GLsizei w, GLsizei h)
{
    CurrentWidth = w, CurrentHeight = h;
    glViewport((w - Width) / 2, (h - Height) / 2, Width, Height);
}

bool alt_key;
bool ctrl_key;

void keyboard(int key, int x, int y)
{
    if (key == 033) {
        if (my_scene.transformation_is_active())
            my_scene.deactivate_transformation();
        else
            exit(EXIT_SUCCESS);
    }

    if (key == 'v')
        my_scene.toogle_vertex_normals();
//    if (key == 'f')
//        my_mesh_1.toogle_face_normals();
    if (key == 'b')
        my_scene.toogle_bounding_box();
    if (key == '[')
        my_scene.previous_camera();
    if (key == ']')
        my_scene.next_camera();
    if (key == 'c')
        my_scene.add_camera();
    if (key == 'd')
        my_scene.delete_active_camera();
    if (key == '9')
        my_scene.previous_object();
    if (key == '0')
        my_scene.next_object();
    if (key == 'w')
        my_scene.activate_translation();
    if (key == 'r')
        my_scene.activate_scaling();
    if (key == 'e')
        my_scene.activate_rotation();

    glutPostRedisplay();
}


bool left_button = false;
bool right_button = false;
bool middle_button = false;

int axis = -1;

int x_prev, y_prev;             // Previous pointer's coordinates

void mouse(int button, int state, int x, int y)
{
    glutGetModifiers() == GLUT_ACTIVE_ALT ? alt_key = true : alt_key = false;
    glutGetModifiers() == GLUT_ACTIVE_CTRL ? ctrl_key = true : ctrl_key = false;

    if (state == GLUT_DOWN) {
        x_prev = x;
        y_prev = y;

        if (button == GLUT_LEFT_BUTTON)
            left_button = true;
        if (button == GLUT_RIGHT_BUTTON)
            right_button = true;
        if (button == GLUT_MIDDLE_BUTTON)
            middle_button = true;

    } else {
        axis = -1;
        left_button = false;
        right_button = false;
        middle_button = false;
    }
}

void mouse_motion(int x, int y)
{
    int delta_x = x_prev - x;
    int delta_y = y_prev - y;

    x_prev = x;
    y_prev = y;

    // Spherical rotation
    if (alt_key && left_button)
        my_scene.update_camera_spherical(delta_x, delta_y);
    // Camera roll
    else if (ctrl_key && left_button)
        my_scene.update_camera_roll(delta_x);
    // Camera zoom (move along camera's main axis)
    else if (alt_key && right_button)
        my_scene.update_camera_zoom(-delta_x - delta_y);
    // Camera move (scan move) in the main plane
    else if (alt_key && middle_button)
        my_scene.update_camera_move(delta_x, delta_y);
    // Transformation handler
    else if (my_scene.transformation_is_active() && left_button)
        axis = my_scene.local_transform(
            axis,
            (double) -delta_x / 300,
            (double)  delta_y / 300,
            (-CurrentWidth + 2 * x) / (double) Width,
            (CurrentHeight - 2 * y) / (double) Height);

    glutPostRedisplay();
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
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_motion);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Init(argc, argv);

    glutMainLoop();

    return 0;
}
