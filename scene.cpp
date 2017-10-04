#include "scene.hpp"

Scene::Scene()
{
    build_grid();
    build_camera_model();
    build_move_controller();
    build_rot_controller();

    camera_index_ = -1;
    active_camera_ = Camera(vec3(0, 0, 0), vec3(0, 0, 0));

    object_index_ = 0;

    grid_colour_ = vec4(42 / 255.0, 161 / 255.0, 152 / 255.0, 0.5);
    camera_colour_ = vec4(133 / 255.0, 153 / 255.0, 0 / 255.0, 1.0);

    move_s = 0.005;
    rot_s = 0.01;
    zoom_s = 0.01;

    uniform_scaling_ = true;
    active_transform_ = Transformation::disabled;
}

Scene::~Scene()
{
    for(cam_geo_.set_iterator(); cam_geo_.iterator(); cam_geo_.iterate())
        glDeleteBuffers(1, &cam_geo_.get_iterator());

    glDeleteBuffers(1, &grid_buf_);
    glDeleteBuffers(1, &move_controller_buf_);
    glDeleteBuffers(1, &rot_controller_buf_);
    glDeleteVertexArrays(1, &vao_);
}

void Scene::init(GLuint colour, GLuint cam_transform, GLuint local_transform)
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &grid_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, grid_buf_);
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(vec3), grid_, GL_STATIC_DRAW);

    glGenBuffers(1, &move_controller_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, move_controller_buf_);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(vec3), move_controller_,
        GL_STATIC_DRAW);

    glGenBuffers(1, &rot_controller_buf_);
    glBindBuffer(GL_ARRAY_BUFFER, rot_controller_buf_);
    glBufferData(GL_ARRAY_BUFFER, 75 * sizeof(vec3), rot_controller_,
        GL_STATIC_DRAW);

    colour_ = colour;
    cam_transform_ = cam_transform;
    local_transform_ = local_transform;
}

void Scene::add_direct(const char *obj_file, const ColorScheme & colorscheme)
{
    Mesh new_mesh;
    objects_.push(new_mesh);

    if (objects_.length() > 0)
        objects_[object_index_].active = false;

    objects_.tail().load_file(obj_file);
    objects_.tail().set_colorscheme(colorscheme);
    objects_.tail().set_attributes(colour_, local_transform_);

    object_index_ = objects_.length() - 1;
    objects_[object_index_].active = true;
}


void Scene::add_object(const Mesh & G) {
    objects_.push(G);

    if (objects_.length() > 0)
        objects_[object_index_].active = false;

    object_index_ = objects_.length() - 1;
    objects_[object_index_].active = true;
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
            Ry(active_camera_.t[1][1]) *
            Rx(active_camera_.t[1][0]) *
            Rz(active_camera_.t[1][2]) * camera_model_[i] +
            active_camera_.t[0];

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
    draw_active_controller();
}

void Scene::toogle_vertex_normals()
{
    if (objects_.length() == 0)
        return;

    objects_[object_index_].toogle_vertex_normals();
}

void Scene::toogle_bounding_box()
{
    if (objects_.length() == 0)
        return;

    objects_[object_index_].toogle_bounding_box();
}

// Maya controls

void Scene::update_camera_move(int delta_x, int delta_y) {
    active_camera_.t[0] =
    Rz(-active_camera_.t[1][2]) *
    Rx(-active_camera_.t[1][0]) *
    Ry(-active_camera_.t[1][1]) *
    active_camera_.t[0];

    active_camera_.t[0][1] -= move_s * delta_y;
    active_camera_.t[0][0] += move_s * delta_x;

    active_camera_.t[0] =
    Ry(active_camera_.t[1][1]) *
    Rx(active_camera_.t[1][0]) *
    Rz(active_camera_.t[1][2]) *
    active_camera_.t[0];

    camera_index_ = -1;
}

void Scene::update_camera_roll(int d) {
    active_camera_.t[1][2] += rot_s * d;

    if (active_camera_.t[1][2] > 2 * pi)
        active_camera_.t[1][2] -= 2 * pi;
    if (active_camera_.t[1][2] < - 2 * pi)
        active_camera_.t[1][2] += 2 * pi;

    camera_index_ = -1;
}

void Scene::update_camera_zoom(int d) {
    vec3 v(0,0,1);
    v = Ry(active_camera_.t[1][1]) * Rx(active_camera_.t[1][0]) * v;
    active_camera_.t[0] += zoom_s * d * v;
    camera_index_ = -1;
}

void Scene::update_camera_spherical(int dx, int dy) {
    active_camera_.t[0] =
        Ry( rot_s * dx + active_camera_.t[1][1]) *
        Rx(-rot_s * dy) *
        Ry(-active_camera_.t[1][1]) *
        active_camera_.t[0];

    active_camera_.t[1][0] -= rot_s * dy;
    active_camera_.t[1][1] += rot_s * dx;

    if (active_camera_.t[1][1] > 2 * pi)
        active_camera_.t[1][1] -= 2 * pi;
    if (active_camera_.t[1][1] < - 2 * pi)
        active_camera_.t[1][1] += 2 * pi;

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

    glDeleteBuffers(1, & cam_geo_[camera_index_]);
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

void Scene::build_move_controller()
{
    move_controller_[0] = vec3(0.0, 0.0, 0.0);
    move_controller_[1] = vec3(0.3, 0.0, 0.0);

    move_controller_[2] = vec3(0.0, 0.0, 0.0);
    move_controller_[3] = vec3(0.0, 0.3, 0.0);

    move_controller_[4] = vec3(0.0, 0.0, 0.0);
    move_controller_[5] = vec3(0.0, 0.0, 0.3);
}

void Scene::build_rot_controller()
{
    unsigned int i = 0, n = 25;

    // x-axis
    for (double t = 0; t < 2 * pi; t += 2 * pi / n)
        rot_controller_[i++] = 0.2 * vec3(0, cos(t), sin(t));

    // y-axis
    for (double t = 0; t < 2 * pi; t += 2 * pi / n)
        rot_controller_[i++] = 0.2 * vec3(cos(t), 0, sin(t));

    // z-axis
    for (double t = 0; t < 2 * pi; t += 2 * pi / n)
        rot_controller_[i++] = 0.2 * vec3(cos(t), sin(t), 0);
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

void Scene::draw_active_controller()
{
    if (objects_.length() == 0 || active_transform_ == Transformation::disabled)
        return;

    mat4 pivot_transform = Translate(objects_[object_index_].pivot);

    glUniformMatrix4fv(local_transform_, 1, true, (GLfloat*) & pivot_transform);

    // Translation
    if (active_transform_ == Transformation::translation) {
        vec4 tmp_colour;

        glBindBuffer(GL_ARRAY_BUFFER, move_controller_buf_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glLineWidth(2);

        tmp_colour = vec4(1, 0, 0, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_LINES, 0, 2);

        tmp_colour = vec4(0, 1, 0, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_LINES, 2, 2);

        tmp_colour = vec4(0, 0, 1, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_LINES, 4, 2);

        glLineWidth(1);
    }

    // Scaling
    if (active_transform_ == Transformation::scaling) {
        vec4 tmp_colour;

        glBindBuffer(GL_ARRAY_BUFFER, move_controller_buf_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glLineWidth(2);
        glPointSize(8);

        tmp_colour = vec4(1, 0, 0, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_POINTS, 1, 1);
        glDrawArrays(GL_LINES, 0, 2);

        tmp_colour = vec4(0, 1, 0, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_POINTS, 3, 1);
        glDrawArrays(GL_LINES, 2, 2);

        tmp_colour = vec4(0, 0, 1, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_POINTS, 5, 1);
        glDrawArrays(GL_LINES, 4, 2);

        tmp_colour = vec4(1, 1, 1, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_POINTS, 0, 1);

        glPointSize(1);
        glLineWidth(1);
    }

    // Rotation
    if (active_transform_ == Transformation::rotation) {
        vec4 tmp_colour;

        glBindBuffer(GL_ARRAY_BUFFER, rot_controller_buf_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glLineWidth(2);
        glPointSize(8);

        tmp_colour = vec4(1, 0, 0, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_LINE_LOOP, 0, 25);

        tmp_colour = vec4(0, 1, 0, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_LINE_LOOP, 25, 25);

        tmp_colour = vec4(0, 0, 1, 1);
        glUniform4fv(colour_, 1, (GLfloat*) & tmp_colour);
        glDrawArrays(GL_LINE_LOOP, 50, 25);

        glPointSize(1);
        glLineWidth(1);
    }
}

void Scene::use_camera(Camera cam) {
    glUniform3fv(cam_transform_, 2, (GLfloat*) & cam);
}

void Scene::previous_object()
{
    if (objects_.length() == 0)
        return;

    if (objects_.length() > 1)
        objects_[object_index_].active = false;

    object_index_--;
    if (object_index_ < 0)
        object_index_ = objects_.length() - 1;

    objects_[object_index_].active = true;
}

void Scene::next_object()
{
    if (objects_.length() == 0)
        return;

    if (objects_.length() > 1)
        objects_[object_index_].active = false;

    object_index_++;
    if (object_index_ >= objects_.length())
        object_index_ = 0;

    objects_[object_index_].active = true;
}

void Scene::activate_translation()
{
    active_transform_ = Transformation::translation;
}

void Scene::activate_scaling()
{
    active_transform_ = Transformation::scaling;
}

void Scene::activate_rotation()
{
    active_transform_ = Transformation::rotation;
}

void Scene::deactivate_transformation()
{
    active_transform_ = Transformation::disabled;
}

bool Scene::transformation_is_active()
{
    return (active_transform_ == Transformation::disabled) ? false : true;
}

vec2 Scene::camera_plane_projection(vec3 point)
{
    vec3 a = active_camera_.t[1];
    vec3 p = Rz(-a.z) * Rx(-a.x) * Ry(-a.y) * (point - active_camera_.t[0]);
    return vec2(p.x, p.y);
}

void Scene::axis_transform(unsigned int axis, double delta_x, double delta_y)
{
    mat4 t;

//    current_object.transformation = t * current_object.transformation;

    if (uniform_scaling_ && active_transform_ == Transformation::scaling) {
        t = mat4(
            1 + delta_x + delta_y, 0, 0, 0,
            0, 1 + delta_x + delta_y, 0, 0,
            0, 0, 1 + delta_x + delta_y, 0,
            0, 0, 0, 1
        );

        t = Translate(objects_[object_index_].pivot) *
            t * Translate(-objects_[object_index_].pivot);

        objects_[object_index_].transformation =
            t * objects_[object_index_].transformation;
        return;
    }


    if (active_transform_ == Transformation::rotation) {
        switch (axis) {
            case 0:
                t = RotX(delta_x + delta_y);
                break;
            case 1:
                t = RotY(delta_x + delta_y);
                break;
            case 2:
                t = RotZ(delta_x + delta_y);
                break;
        }

        t = Translate(objects_[object_index_].pivot) *
            t * Translate(-objects_[object_index_].pivot);

        objects_[object_index_].transformation =
            t * objects_[object_index_].transformation;

        return;
    }


    vec2 p = camera_plane_projection(objects_[object_index_].pivot);

    vec2 end_p;

    end_p = camera_plane_projection(objects_[object_index_].pivot +
        move_controller_[2 * axis + 1]);

    vec2 dv = vec2(delta_x, delta_y);

    double delta_plane = dot(end_p - p, dv) / length(end_p - p);

    vec3 cam_vec =
        Ry(active_camera_.t[1][1]) *
        Rx(active_camera_.t[1][0]) * vec3(0, 0, 1);

    double C = dot(cam_vec, move_controller_[2 * axis + 1]) /
        length(move_controller_[2 * axis + 1]);

    if (C == 1)
        return;

    double d_axis = delta_plane / sqrt(1 - C * C);

    if (active_transform_ == Transformation::translation) {
        switch (axis) {
            case 0:
                objects_[object_index_].pivot.x += d_axis;
                t = Translate(d_axis, 0, 0);
                break;
            case 1:
                objects_[object_index_].pivot.y += d_axis;
                t = Translate(0, d_axis, 0);
                break;
            case 2:
                objects_[object_index_].pivot.z += d_axis;
                t = Translate(0, 0, d_axis);
                break;
        }

        objects_[object_index_].transformation =
            t * objects_[object_index_].transformation;
    }
    // else if (active_transform_ == Transformation::scaling)
    //    objects_[object_index_].transform[2][axis] += d_axis;
}

int Scene::local_transform(int axis, double delta_x, double delta_y,
    double x, double y)
{

    // Translating and scaling
    if (active_transform_ == Transformation::translation ||
        active_transform_ == Transformation::scaling) {

        // Check if currently there is nothing to transform
        if (axis == -1) {
            // Find projections to the camera plane
            vec2 p = camera_plane_projection(
                objects_[object_index_].pivot);
            vec2 end_p;

            uniform_scaling_ = true;

            // Check "closest" axis
            for (int i = 0; i < 3; i++) {
                end_p = camera_plane_projection(
                    objects_[object_index_].pivot +
                    move_controller_[2 * i + 1]);

                if (length(end_p - p) <= 0.05 && i == 2) {
                    axis = 2;
                    continue;
                }

                if (belongs_to_segment(vec2(x, y), p, end_p, 0.05)) {
                    axis = i;
                    break;
                }
            }

            // Uniform scaling check
            uniform_scaling_ = true;
            if (active_transform_ == Transformation::scaling)
                for (int i = 0; i < 3; i++) {
                    end_p =
                        camera_plane_projection(
                            objects_[object_index_].pivot +
                            move_controller_[2 * i + 1]);

                    if (!belongs_to_segment(vec2(x, y), p, end_p, 0.05)) {
                        uniform_scaling_ = false;
                        break;
                    }
                }
        }

        if (axis == -1)
            return -1;

        axis_transform(axis, delta_x, delta_y);

        return axis;
    }

    // Rotation
    if (active_transform_ == Transformation::rotation) {

        if (axis == -1) {
            // Find projections to the camera plane
            vec2 p;

            uniform_scaling_ = true;

            // Check "closest" axis
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 25; j++) {
                    p = camera_plane_projection(
                        objects_[object_index_].pivot +
                        rot_controller_[i * 25 + j]);

                    if (sqrt(dot(p -  vec2(x, y), p - vec2(x, y))) <= 0.05) {
                        axis = i;
                        axis_transform(axis, delta_x, delta_y);
                        return axis;
                        break;
                    }
                }
        }

        if (axis == -1)
            return -1;

        axis_transform(axis, delta_x, delta_y);

        return axis;
    }

    return -1;
}
