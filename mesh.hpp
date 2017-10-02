#ifndef MESH_HPP
#define MESH_HPP

#include <fstream>
#include <sstream>
#include <string>

#include "colorscheme.hpp"
#include "graphics_root.hpp"
#include "vec.hpp"
#include "list.hpp"

class Mesh {

    //
    // Data
    //

    // Faces and vertex normals (with respect to faces)
    // Note: the size of f_ is 3 * f_number_ and vn_ is of 6 * f_number_
    vec3 *f_, *vn_;
    int f_number_;

    // Bounding box in local coordinates
    vec3 bounding_box_[24];

    // Shader attributes: color is used for rendering all the parts of
    // geometry, that is edges, normals and bounding box, transform_loc_
    // represents the location of local transform 4 by 4 matrix
    GLuint colour_;

    // Array of colors, used as color scheme
    ColorScheme colorscheme_;

    // Check wheter to render: vertex normals, face normals, bounding box
    bool draw_mode_[3];

    // Main vertex buffer: faces, bounding box and vertex normals
    GLuint mesh_vbo_;

    GLuint local_transform_;

    //
    // Private functions
    //

    // Build bounding box by 6 bounding planes
    void build_box(GLfloat box_limit[6]);

    // Initialise mesh_vbo_
    void set_main_buffer();

public:

    // Local transformation: 0 - position, 1 - rotation, 2 - scaling
    vec3 transform[3];
    // Pivot, intialised as geometric center at the beginning
    vec3 pivot;


    Mesh();

    // Note: this method creates new buffer object
    Mesh(const Mesh& mesh);

    ~Mesh();

    // Read .obj file (works only with triangles)
    void load_file(const char *obj_file);

    // Set colorscheme defined in colorscheme.hpp
    void set_colorscheme(const ColorScheme & colorscheme);

    void set_attributes(GLuint color_location, GLuint local_transform);

    // Render geometry
    void draw();

    // Toogle rendering of dfferent elements
    void toogle_vertex_normals();
    void toogle_face_normals();
    void toogle_bounding_box();

    // Model transformation

    bool active;
};

#endif
