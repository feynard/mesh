attribute vec4 vertex_position;

// Camera transformation
uniform mat4 camera;

//uniform vec3 pivot;
uniform mat4 local_transformation;

void main()
{
    mat4 ClipScale = mat4(
        0.1, 0.0, 0.0, 0.0,
        0.0, 0.1, 0.0, 0.0,
        0.0, 0.0, 0.1, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 Perspective = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, -1.0,
        0.0, 0.0, 0.0, 0.0
    );

    gl_Position = Perspective * camera * local_transformation * vertex_position;
}
