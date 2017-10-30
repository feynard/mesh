#ifndef SCENE_HPP
#define SCENE_HPP

#include "graphics_root.hpp"
#include "vec.hpp"
#include "mat.hpp"
#include "mesh.hpp"
#include "list.hpp"

class Scene {

    // Inner camera structure
    struct Camera {
        Camera() { t[0] = 0, t[1] = 0; }
        Camera(vec3 pos, vec3 rot) { t[0] = pos, t[1] = rot; }

        // 0 - position, 1 - rotation
        mat4 projection;
        vec3 t[2];
    };

    List <Mesh> objects_;       // Main geometry of a scene, that is .obj files
    List <Camera> cameras_;     // All the cameras
    List <GLuint> cam_geo_;     // Geometry buffers of all the cameras

    // Active mesh, switching bounding box and normals works for this object
    int object_index_;

    // Active camera and index for tracing through the all cameras (in a list)
    Camera active_camera_;
    int camera_index_;

    // Shader attributes
    GLuint color_, cam_transform_, local_transform_;

    // Grid (Maya-like)
    GLuint grid_buf_;
    vec3 grid_[36];
    vec4 grid_color_;

    // Model of camera
    vec3 camera_model_[48];
    vec4 camera_color_;

    // Transformation controllers
    enum class Transformation {
        disabled, translation, scaling, uniform_scaling, rotation
    };

    Transformation active_transform_;

    vec3 move_controller_[18];       // Also used for drawing scaling controller
    vec3 rot_controller_[75];

    GLuint move_controller_buf_;    // Again, used also for scaling controller
    GLuint rot_controller_buf_;

    // Mouse sensitivity
    GLfloat move_s;
    GLfloat rot_s;
    GLfloat zoom_s;

    // Main vertex array object (VAO)
    GLuint vao_;

public:

    // It's important to not to do anything with GL here
    Scene();
    ~Scene();

    // Default initiliser, need to prevent segmentation fault errors (GL
    // functions can't be called before GLUT is initialised)
    void init(GLuint color, GLuint cam_transform, GLuint local_transform);

    // Add new object
    void add_object(const Mesh & G);

    // Add new object without calling copy constructor
    void add_direct(const char *obj_file, const ColorScheme & colorscheme);

    // Add new camera
    void add_camera(vec3 pos, vec3 rot);

    // Add active camera to the cameras list
    void add_camera();

    // Main draw callback
    void draw();

    // Camera move (Maya-like)
    void update_camera_move(int delta_x, int delta_y);

    // Update current camera's roll
    void update_camera_roll(int d);

    // Zoom for current camera (that is moving camera along its main axis)
    void update_camera_zoom(int d);

    // Spherical rotation of a camera (Maya-like) about origin
    void update_camera_spherical(int dx, int dy);

    // Switch between cameras
    void previous_camera();
    void next_camera();

    // Switch between objects
    void previous_object();
    void next_object();

    // Toogle drawing options for the active object
    void toogle_vertex_normals();
    void toogle_bounding_box();

    // If camera was set by the index in cameras array it will be deleted
    void delete_active_camera();

    // Transformation setters
    void activate_translation();
    void activate_scaling();
    void activate_rotation();

    void deactivate_transformation();

    // Check if there is some transfomration in use now
    bool transformation_is_active();

    // Deltas are pointer speed parameters, x and y are screen coordinates,
    // that is normalised to be in interval [-1, 1], right bottom corner of a
    // viewport corresponds to (-1, -1)
    int local_transform(int axis, double delta_x, double delta_y,
        double x, double y);

private:
    // Geometry for scene objects
    void build_camera_model();
    void build_grid();
    void build_move_controller();
    void build_rot_controller();

    // Calculate a parallel projection of a point to the screen plane
    vec2 camera_plane_projection(vec3 point);

    // Translates object along the axis according to the speed of pointer
    void axis_transform(unsigned int axis, double delta_x, double delta_y);

    // Drawing functions
    void draw_grid();
    void draw_cameras();
    void draw_active_controller();

    // Send camera transfomration to shader
    void use_camera(Camera cam);
};

#endif
