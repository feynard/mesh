#include "mesh.hpp"

class Scene {

    struct Camera {
        Camera(vec3 pos, vec3 rot) : position(pos), rotation(rot) {};
        vec3 position;
        vec3 rotation;
    };

    List <Mesh*> objects_;
    List <Camera> cameras_;

    // Shader attributes
    GLuint cam_pos_, cam_rot_, color_;

    GLuint grid_buf_;
    vec3 grid_[22];
    vec4 grid_color_;

public:

    Scene() {
        grid_[0] = vec3(-0.5, 0, -0.5);
        grid_[1] = vec3(-0.5, 0, 0.5);
        grid_[2] = vec3(-0.25, 0, -0.5);
        grid_[3] = vec3(-0.25, 0, 0.5);
        grid_[4] = vec3(0, 0, -0.5);
        grid_[5] = vec3(0, 0, 0.5);
        grid_[6] = vec3(0.25, 0, -0.5);
        grid_[7] = vec3(0.25, 0, 0.5);
        grid_[8] = vec3(0.5, 0, -0.5);
        grid_[9] = vec3(0.5, 0, 0.5);

        grid_[10] = vec3(-0.5, 0, -0.5);
        grid_[11] = vec3(0.5, 0, -0.5);
        grid_[12] = vec3(-0.5, 0, -0.25);
        grid_[13] = vec3(0.5, 0, -0.25);
        grid_[14] = vec3(-0.5, 0, 0);
        grid_[15] = vec3(0.5, 0, 0);
        grid_[16] = vec3(-0.5, 0, 0.25);
        grid_[17] = vec3(0.5, 0, 0.25);
        grid_[18] = vec3(-0.5, 0, 0.5);
        grid_[19] = vec3(0.5, 0, 0.5);

        grid_[20] = vec3(-0.25, 0, -0.25);
        grid_[21] = vec3(-0.25, 0.5, -0.25);

        grid_color_ = vec4(42 / 255.0, 161 / 255.0, 152 / 255.0, 1.0);
    }

    void init(GLuint color, GLuint camera_position, GLuint camera_rotation) {
        color_ = color;
        cam_pos_ = camera_position;
        cam_rot_ = camera_rotation;

        glGenBuffers(1, &grid_buf_);
        glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
        glBufferData(GL_ARRAY_BUFFER, 22 * sizeof(vec3), grid_, GL_STATIC_DRAW);
    }

    void add_object(Mesh* G) {
        objects_.push(G);
    }

    // Add new camera
    void add_camera(vec3 pos, vec3 rot) {
        cameras_.push(Camera(pos, rot));
    }

    void use_camera(int i) {
        glUniform3fv(cam_pos_, 1, (GLfloat*) &cameras_[i].position);
        glUniform3fv(cam_rot_, 1, (GLfloat*) &cameras_[i].rotation);
    }

    void draw() {
        use_camera(0);

        for(objects_.set_iterator(); objects_.iterator(); objects_.iterate()) {
            objects_.get_iterator() -> draw();
        }

        draw_grid();
    }

    void draw_grid() {
        glUniform4fv(color_, 1, (GLfloat*) &grid_color_);
        glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINES, 0, 22);
    }
};
