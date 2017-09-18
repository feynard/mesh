attribute vec4 position;

// Debug animation
uniform float theta;

// Camera controls
uniform vec4 cam_position;
uniform vec4 cam_rotation;

float pi = 3.14159265358979323846;

void main() {

    float C = cos(theta);
    float S = sin(theta);

    mat4 Ry = mat4(
           C,  0.0,   -S,  0.0,
         0.0,  1.0,  0.0,  0.0,
           S,  0.0,    C,  0.0,
         0.0,  0.0,  0.0,  1.0
    );


    C = cos(-cam_rotation[0]);
    S = sin(-cam_rotation[0]);
    mat4 yaw = mat4(
           C,  0.0,   -S,  0.0,
         0.0,  1.0,  0.0,  0.0,
           S,  0.0,    C,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    C = cos(-cam_rotation[1]);
    S = sin(-cam_rotation[1]);
    mat4 pitch = mat4(
         1.0,  0.0,  0.0,  0.0,
         0.0,    C,   -S,  0.0,
         0.0,    S,    C,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    C = cos(-cam_rotation[2]);
    S = sin(-cam_rotation[2]);
    mat4 roll = mat4(
           C,   -S,  0.0,  0.0,
           S,    C,  0.0,  0.0,
         0.0,  0.0,  1.0,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    vec4 tmp_position = Ry * position;

    tmp_position -= cam_position;

    gl_Position = pitch * yaw * roll * tmp_position;
}
