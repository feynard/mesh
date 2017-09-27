#ifndef SCENE_HPP
#define SCENE_HPP

#include "graphics_root.hpp"
#include "vec.hpp"
#include "mat.hpp"
#include "mesh.hpp"
#include "list.hpp"

class Scene {

    // Inner camera structure for holding transformations
    struct Camera {
        Camera() : position(0), rotation(0) {}
        Camera(vec3 pos, vec3 rot) : position(pos), rotation(rot) {}

        vec3 position;
        vec3 rotation;
    };

    List <Mesh> objects_;      // Main geometry of a scene: .obj files, etc.
    List <Camera> cameras_;     // All the cameras
    List <GLuint> cam_geo_;     // Geometry buffers of all the cameras

    // Active camera and index for tracing through the all cameras (in a list)
    Camera active_camera_;
    int camera_index_;

    // Shader attributes
    GLuint cam_pos_, cam_rot_, colour_;

    // Grid (Maya-like)
    GLuint grid_buf_;
    vec3 grid_[36];
    vec4 grid_colour_;

    // Model of camera
    vec3 camera_model_[48];
    vec4 camera_colour_;

    // Mouse sensitivity
    GLfloat move_s;
    GLfloat rot_s;
    GLfloat zoom_s;

    // Vertex array object (VAO)
    GLuint vao_;

public:

    // It's important to not to do anything with GL here
    Scene();
    ~Scene();

    // Default initiliser, need to prevent segmentation fault errors (GL
    // functions can't be called before GLUT is initialised)
    void init(GLuint colour, GLuint cam_position, GLuint cam_rotation);

    // Add new object
    // New version (with new list):
    // add_object(Mesh& mesh) {
    //     objects_.push(mesh);
    // }
    void add_object(const Mesh & G);

    // Add new camera
    void add_camera(vec3 pos, vec3 rot);

    // Add active camera to the cameras list
    void add_camera();

    // Main draw callback
    void draw();

    // Camera move
    void update_camera_move(int delta_x, int delta_y);

    // Update current camera's roll
    void update_camera_roll(int d);

    // Zoom for current camera (that is moving camera along its main axis)
    void update_camera_zoom(int d);

    // Spherical rotation of a camera (Maya-like)
    void update_camera_spherical(int dx, int dy);

    // Switch between cameras
    void previous_camera();

    // Switch between cameras
    void next_camera();

    // If camera was set by the index in cameras array it will be deleted
    void delete_active_camera();

private:
    void build_camera_model();
    void build_grid();
    void draw_grid();
    void draw_cameras();
    void use_camera(Camera cam);
};

#endif
