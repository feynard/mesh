attribute vec4 vPosition;
uniform float theta;

float pi = 3.14159265358979323846;

void main() {
    float C = cos(theta);
    float S = sin(theta);

    mat4 Ry = mat4 (
           C,  0.0,   -S,  0.0,
		 0.0,  1.0,  0.0,  0.0,
		   S,  0.0,    C,  0.0,
		 0.0,  0.0,  0.0,  1.0
    );

    C = cos(pi / 6.0);
    S = sin(pi / 6.0);

    mat4 Rx = mat4 (
         1.0,  0.0,  0.0,  0.0,
         0.0,    C,   -S,  0.0,
         0.0,    S,    C,  0.0,
         0.0,  0.0,  0.0,  1.0
    );

    gl_Position = Rx * Ry * vPosition;
}
