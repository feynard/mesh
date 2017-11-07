#version 410

in vec4 vertex_position;

// Camera transformation
uniform mat4 camera;

// Local transformation
uniform mat4 local_transformation;

void main()
{
    gl_Position =
        camera * local_transformation * vertex_position / vertex_position.w;
}
