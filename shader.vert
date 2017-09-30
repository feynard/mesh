attribute vec4 vertex_position;

// Camera transformation: cam[0] - position, cam[1] - rotation
uniform vec3 cam[2];

// Local transformation: loc[0] - position, loc[1] - rotation
uniform vec3 loc[2];


float pi = 3.14159265358979323846;

void main() {

    //
    // Camera transformation
    //

    // Rotations

    float C;
    float S;

    C = cos(cam[1][0]);
    S = sin(cam[1][0]);
    mat4 Rx = mat4(
         1.0,  0.0,  0.0,  0.0,
         0.0,    C,   -S,  0.0,
         0.0,    S,    C,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    C = cos(cam[1][1]);
    S = sin(cam[1][1]);
    mat4 Ry = mat4(
           C,  0.0,   -S,  0.0,
         0.0,  1.0,  0.0,  0.0,
           S,  0.0,    C,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    C = cos(cam[1][2]);
    S = sin(cam[1][2]);
    mat4 Rz = mat4(
           C,   -S,  0.0,  0.0,
           S,    C,  0.0,  0.0,
         0.0,  0.0,  1.0,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    // Translation

    mat4 T = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -cam[0][0], -cam[0][1], -cam[0][2], 1.0
    );

    // Resulting matrix

    mat4 cam_transformation = Rz * Rx * Ry * T;

    //
    // Local transformation
    //

    gl_Position = cam_transformation * vertex_position;

}
