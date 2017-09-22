#include "mesh.hpp"

class Scene {

    struct Camera {
        Camera() : position(vec3(0)), rotation(vec3(0)) {}
        Camera(vec3 pos, vec3 rot) : position(pos), rotation(rot) {}

        vec3 position;
        vec3 rotation;
    };

    List <Mesh*> objects_;
    List <Camera> cameras_;

    Camera active_camera_;
    int camera_index_;

    // Shader attributes
    GLuint cam_pos_, cam_rot_, color_;

    GLuint grid_buf_;
    vec3 grid_[36];
    vec4 grid_color_;

public:

    Scene() {

        for (int i = 0; i < 9; i++) {
            grid_[2 * i] = vec3(-1 + i * 0.25, 0, -1);
            grid_[2 * i + 1] = vec3(-1 + i * 0.25, 0, 1);
        }

        for (int i = 0; i < 9; i++) {
            grid_[18 + 2 * i] = vec3(-1, 0, -1 + i * 0.25);
            grid_[19 + 2 * i] = vec3(1, 0, -1 + i * 0.25);
        }

        camera_index_ = 0;
        active_camera_ = Camera(vec3(0), vec3(0));

        grid_color_ = vec4(42 / 255.0, 161 / 255.0, 152 / 255.0, 1.0);
    }

    void init(GLuint color, GLuint camera_position, GLuint camera_rotation) {
        color_ = color;
        cam_pos_ = camera_position;
        cam_rot_ = camera_rotation;

        glGenBuffers(1, &grid_buf_);
        glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
        glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(vec3), grid_, GL_STATIC_DRAW);
    }

    void add_object(Mesh* G) {
        objects_.push(G);
    }

    // Add new camera
    void add_camera(vec3 pos, vec3 rot) {
        cameras_.push(Camera(pos, rot));
        camera_index_ = cameras_.length() - 1;
        active_camera_ = Camera(pos, rot);
    }

    void add_camera() {
        cameras_.push(active_camera_);
        camera_index_ = cameras_.length() - 1;
    }

private:

    void draw_grid() {
        glUniform4fv(color_, 1, (GLfloat*) &grid_color_);
        glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINES, 0, 36);
    }

    void use_camera(Camera cam) {
        glUniform3fv(cam_pos_, 1, (GLfloat*) &cam.position);
        glUniform3fv(cam_rot_, 1, (GLfloat*) &cam.rotation);
    }

public:

    void draw() {
        use_camera(active_camera_);

        for(objects_.set_iterator(); objects_.iterator(); objects_.iterate())
            objects_.get_iterator() -> draw();



        draw_grid();
//        draw_cameras();
    }

    void update_camera_roll(int d) {
        active_camera_.rotation[2] += 0.01 * d;

        if (active_camera_.rotation[2] > 2 * pi)
            active_camera_.rotation[2] -= 2 * pi;
        if (active_camera_.rotation[2] < - 2 * pi)
            active_camera_.rotation[2] += 2 * pi;

        camera_index_ = -1;
    }

    void update_camera_rotation(int dx, int dy) {

        active_camera_.position =
            Ry(-0.01 * dx - active_camera_.rotation.y) *
            Rx(-0.01 * dy) *
            Ry(active_camera_.rotation.y) *
            active_camera_.position;

        active_camera_.rotation[0] -= 0.01 * dy;
        active_camera_.rotation[1] += 0.01 * dx;

        if (active_camera_.rotation[1] > 2 * pi)
            active_camera_.rotation[1] -= 2 * pi;
        if (active_camera_.rotation[1] < - 2 * pi)
            active_camera_.rotation[1] += 2 * pi;

        camera_index_ = -1;
    }

    void previous_camera() {
        if (camera_index_ == -1) {
            camera_index_ = 0;
            active_camera_ = *cameras_[camera_index_];
            return;
        }

        if (camera_index_ == 0)
            camera_index_ = cameras_.length() - 1;
        else
            camera_index_--;

        active_camera_ = *cameras_[camera_index_];
    }

    void next_camera() {
        if (camera_index_ == -1) {
            camera_index_ = 0;
            active_camera_ = *cameras_[camera_index_];
            return;
        }

        if (camera_index_ == cameras_.length() - 1)
            camera_index_ = 0;
        else
            camera_index_++;

        active_camera_ = *cameras_[camera_index_];
    }
};
