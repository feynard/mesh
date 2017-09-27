#include "scene.hpp"

Scene::Scene() {
    build_grid();
    build_camera_model();

    camera_index_ = 0;
    active_camera_ = Camera(vec3(0, 0, 0), vec3(0, 0, 0));

    grid_colour_ = vec4(42 / 255.0, 161 / 255.0, 152 / 255.0, 1.0);
    camera_colour_ = vec4(133 / 255.0, 153 / 255.0, 0 / 255.0, 1.0);

    move_s = 0.005;
    rot_s = 0.01;
    zoom_s = 0.01;
}

Scene::~Scene() {
    for(cam_geo_.set_iterator(); cam_geo_.iterator(); cam_geo_.iterate())
        glDeleteBuffers(1, &cam_geo_.get_iterator());

    glDeleteBuffers(1, &grid_buf_);
    glDeleteVertexArrays(1, &vao_);
}

void Scene::init(GLuint colour, GLuint cam_position, GLuint cam_rotation) {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &grid_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(vec3), grid_, GL_STATIC_DRAW);

    colour_ = colour;
    cam_pos_ = cam_position;
    cam_rot_ = cam_rotation;
}

void Scene::add_object(const Mesh & G) {
    objects_.push(G);
}

void Scene::add_camera(vec3 pos, vec3 rot) {
    cameras_.push(Camera(pos, rot));
    camera_index_ = cameras_.length() - 1;
    active_camera_ = Camera(pos, rot);

    GLuint tmp_buf;

    glGenBuffers(1, &tmp_buf);
    glBindBuffer(GL_ARRAY_BUFFER, tmp_buf);

    vec3 new_camera[48];

    for (int i = 0; i < 48; i++)
        new_camera[i] =
            Ry(rot.y) * Rx(rot.x) * Rz(rot.z) * camera_model_[i] + pos;

    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(vec3), new_camera,
        GL_STATIC_DRAW);

    cam_geo_.push(tmp_buf);
}

void Scene::add_camera() {
    cameras_.push(active_camera_);
    camera_index_ = cameras_.length() - 1;

    // Initilise new buffer
    GLuint tmp_buf;
    glGenBuffers(1, &tmp_buf);
    glBindBuffer(GL_ARRAY_BUFFER, tmp_buf);

    // Transform camera geometry
    vec3 new_camera[48];
    for (int i = 0; i < 48; i++)
        new_camera[i] =
            Ry(active_camera_.rotation.y) *
            Rx(active_camera_.rotation.x) *
            Rz(active_camera_.rotation.z) * camera_model_[i] +
            active_camera_.position;

    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(vec3), new_camera,
        GL_STATIC_DRAW);

    cam_geo_.push(tmp_buf);
}

void Scene::draw() {
    use_camera(active_camera_);

    for(objects_.set_iterator(); objects_.iterator(); objects_.iterate())
        objects_.get_iterator().draw();

    draw_grid();
    draw_cameras();
}

void Scene::update_camera_move(int delta_x, int delta_y) {
    active_camera_.position =
    Rz(-active_camera_.rotation.z) *
    Rx(-active_camera_.rotation.x) *
    Ry(-active_camera_.rotation.y) *
    active_camera_.position;

    active_camera_.position.y -= move_s * delta_y;
    active_camera_.position.x += move_s * delta_x;

    active_camera_.position =
    Ry(active_camera_.rotation.y) *
    Rx(active_camera_.rotation.x) *
    Rz(active_camera_.rotation.z) *
    active_camera_.position;

    camera_index_ = -1;
}void Scene::update_camera_roll(int d) {
    active_camera_.rotation[2] += rot_s * d;

    if (active_camera_.rotation[2] > 2 * pi)
        active_camera_.rotation[2] -= 2 * pi;
    if (active_camera_.rotation[2] < - 2 * pi)
        active_camera_.rotation[2] += 2 * pi;

    camera_index_ = -1;
}
void Scene::update_camera_zoom(int d) {
    vec3 v(0,0,1);
    v = Ry(active_camera_.rotation.y) * Rx(active_camera_.rotation.x) * v;
    active_camera_.position += zoom_s * d * v;
    camera_index_ = -1;
}
void Scene::update_camera_spherical(int dx, int dy) {
    active_camera_.position =
        Ry(rot_s * dx + active_camera_.rotation.y) *
        Rx(-rot_s * dy) *
        Ry(-active_camera_.rotation.y) *
        active_camera_.position;

    active_camera_.rotation[0] -= rot_s * dy;
    active_camera_.rotation[1] += rot_s * dx;

    if (active_camera_.rotation[1] > 2 * pi)
        active_camera_.rotation[1] -= 2 * pi;
    if (active_camera_.rotation[1] < - 2 * pi)
        active_camera_.rotation[1] += 2 * pi;

    camera_index_ = -1;
}

void Scene::previous_camera() {
    if (cameras_.length() == 0)
        return;

    if (camera_index_ == -1) {
        camera_index_ = 0;
        active_camera_ = cameras_[camera_index_];
        return;
    }

    if (camera_index_ == 0)
        camera_index_ = cameras_.length() - 1;
    else
        camera_index_--;

    active_camera_ = cameras_[camera_index_];
}

void Scene::next_camera() {
    if (cameras_.length() == 0)
        return;

    if (camera_index_ == -1) {
        camera_index_ = 0;
        active_camera_ = cameras_[camera_index_];
        return;
    }

    if (camera_index_ == cameras_.length() - 1)
        camera_index_ = 0;
    else
        camera_index_++;

    active_camera_ = cameras_[camera_index_];
}

void Scene::delete_active_camera() {
    if (camera_index_ == -1)
        return;

    glDeleteBuffers(1, &cam_geo_[camera_index_]);
    cameras_.remove_by_index(camera_index_);
    cam_geo_.remove_by_index(camera_index_);
    camera_index_ = -1;
}

void Scene::build_camera_model() {
    camera_model_[0] = vec3(-0.20, -0.15, -0.05);
    camera_model_[1] = vec3( 0.20, -0.15, -0.05);
    camera_model_[2] = vec3(-0.20,  0.15, -0.05);
    camera_model_[3] = vec3( 0.20,  0.15, -0.05);
    camera_model_[4] = vec3(-0.20, -0.15, -0.05);
    camera_model_[5] = vec3(-0.20,  0.15, -0.05);
    camera_model_[6] = vec3( 0.20, -0.15, -0.05);
    camera_model_[7] = vec3( 0.20,  0.15, -0.05);

    camera_model_[8]  = vec3(-0.20, -0.15, 0.05);
    camera_model_[9]  = vec3( 0.20, -0.15, 0.05);
    camera_model_[10] = vec3(-0.20,  0.15, 0.05);
    camera_model_[11] = vec3( 0.20,  0.15, 0.05);
    camera_model_[12] = vec3(-0.20, -0.15, 0.05);
    camera_model_[13] = vec3(-0.20,  0.15, 0.05);
    camera_model_[14] = vec3( 0.20, -0.15, 0.05);
    camera_model_[15] = vec3( 0.20,  0.15, 0.05);

    camera_model_[16] = vec3(-0.20, -0.15, -0.05);
    camera_model_[17] = vec3(-0.20, -0.15,  0.05);
    camera_model_[18] = vec3(-0.20,  0.15, -0.05);
    camera_model_[19] = vec3(-0.20,  0.15,  0.05);
    camera_model_[20] = vec3( 0.20, -0.15, -0.05);
    camera_model_[21] = vec3( 0.20, -0.15,  0.05);
    camera_model_[22] = vec3( 0.20,  0.15, -0.05);
    camera_model_[23] = vec3( 0.20,  0.15,  0.05);

    camera_model_[24] = vec3(-0.07, -0.05, 0.05);
    camera_model_[25] = vec3( 0.07, -0.05, 0.05);
    camera_model_[26] = vec3(-0.07,  0.05, 0.05);
    camera_model_[27] = vec3( 0.07,  0.05, 0.05);
    camera_model_[28] = vec3(-0.07, -0.05, 0.05);
    camera_model_[29] = vec3(-0.07,  0.05, 0.05);
    camera_model_[30] = vec3( 0.07, -0.05, 0.05);
    camera_model_[31] = vec3( 0.07,  0.05, 0.05);

    camera_model_[32] = vec3(-0.10, -0.08, 0.20);
    camera_model_[33] = vec3( 0.10, -0.08, 0.20);
    camera_model_[34] = vec3(-0.10,  0.08, 0.20);
    camera_model_[35] = vec3( 0.10,  0.08, 0.20);
    camera_model_[36] = vec3(-0.10, -0.08, 0.20);
    camera_model_[37] = vec3(-0.10,  0.08, 0.20);
    camera_model_[38] = vec3( 0.10, -0.08, 0.20);
    camera_model_[39] = vec3( 0.10,  0.08, 0.20);

    camera_model_[40] = vec3(-0.07, -0.05, 0.05);
    camera_model_[41] = vec3(-0.10, -0.08, 0.20);
    camera_model_[42] = vec3( 0.07, -0.05, 0.05);
    camera_model_[43] = vec3( 0.10, -0.08, 0.20);
    camera_model_[44] = vec3(-0.07,  0.05, 0.05);
    camera_model_[45] = vec3(-0.10,  0.08, 0.20);
    camera_model_[46] = vec3( 0.07,  0.05, 0.05);
    camera_model_[47] = vec3( 0.10,  0.08, 0.20);

    for (int i = 0; i < 48; i++) {
        camera_model_[i] /= 2;
        camera_model_[i].z *= 2;
    }
}

void Scene::build_grid() {
    for (int i = 0; i < 9; i++) {
        grid_[2 * i]     = vec3(-1 + i * 0.25, 0, -1);
        grid_[2 * i + 1] = vec3(-1 + i * 0.25, 0,  1);
    }

    for (int i = 0; i < 9; i++) {
        grid_[18 + 2 * i] = vec3(-1, 0, -1 + i * 0.25);
        grid_[19 + 2 * i] = vec3( 1, 0, -1 + i * 0.25);
    }
}

void Scene::draw_grid() {
    glUniform4fv(colour_, 1, (GLfloat*) &grid_colour_);
    glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 36);
}

void Scene::draw_cameras() {
    for(cam_geo_.set_iterator(); cam_geo_.iterator(); cam_geo_.iterate()) {
        glUniform4fv(colour_, 1, (GLfloat*) &camera_colour_);
        glBindBuffer(GL_ARRAY_BUFFER, cam_geo_.get_iterator());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINES, 0, 48);
    }
}

void Scene::use_camera(Camera cam) {
    glUniform3fv(cam_pos_, 1, (GLfloat*) &cam.position);
    glUniform3fv(cam_rot_, 1, (GLfloat*) &cam.rotation);
}
