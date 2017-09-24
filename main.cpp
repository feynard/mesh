#include "graphics.hpp"
#include "mesh.hpp"
#include "scene.hpp"

using namespace std;

vec4 color(220 / 255.0, 50 / 255.0, 47 / 255.0, 1.0);

// Shader attribute locations
GLuint CamPos, CamRot, Color, loc;

Mesh my_mesh_1, my_mesh_2;
Scene my_scene;

void Init(int argc, char **argv)
{
    // Load shaders and use the resulting shader program
    GLuint program = ShaderInit("shader.vert", "shader.frag");
    glUseProgram(program);

    // Set shader attributes
    loc    = glGetAttribLocation(program, "position");
    Color  = glGetUniformLocation(program, "edge_color");
    CamPos = glGetUniformLocation(program, "cam_position");
    CamRot = glGetUniformLocation(program, "cam_rotation");

    my_scene.init(Color, CamPos, CamRot);

    if (argc > 1)
        my_mesh_1.load(argv[1]);
    else
        my_mesh_1.load("obj_files/teapot.obj");

    my_mesh_1.set_color_attribute(Color);

    my_scene.add_object(&my_mesh_1);
    my_scene.add_camera(vec3(0.3, 0.3, 0.3), vec3(0.61548, 3 * pi / 4, 0));

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
    "Vertex Normals (v)",
    "Faces Normals (f)",
    "Bounding Box (b)",
    "Switch Between Cameras ([,])",
    "Add Camera (c)",
    "Camera Rotation (Alt + LMB)",
    "Camera Move (Alt + MMB)",
    "Camera Zoom (Alt + RMB)",
    "Camera Roll (Ctrl + LMB)",
    "Delete Camera (d)"
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

bool alt_key;
bool ctrl_key;

void keyboard(int key, int x, int y)
{
    if (key == 033)
        exit(EXIT_SUCCESS);
    if (key == 'v')
        my_mesh_1.toogle_vertex_normals();
    if (key == 'f')
        my_mesh_1.toogle_face_normals();
    if (key == 'b')
        my_mesh_1.toogle_bounding_box();
    if (key == '[')
        my_scene.previous_camera();
    if (key == ']')
        my_scene.next_camera();
    if (key == 'c')
        my_scene.add_camera();
    if (key == 'd')
        my_scene.delete_active_camera();

    glutPostRedisplay();
}


bool left_button = false;
bool right_button = false;
bool middle_button = false;

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

    Init(argc, argv);

    glutMainLoop();

    return 0;
}
