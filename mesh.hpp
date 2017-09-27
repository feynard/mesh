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

    // Faces, vertex normals (with respect to faces)
    vec3 *f_, *vn_;
    int f_number_;

    // Bounding box in local coordinates
    vec3 bounding_box_[24];

    // Shader attributes
    GLuint color_loc_;

    // Array of colors
    ColorScheme colorscheme_;

    // Vertex normals, faces normals, bounding box
    bool draw_mode_[3];

    // Main vertex buffer: faces, bounding box and vertex normals
    GLuint mesh_vbo_;

    //
    // Private functions
    //

    void build_box(GLfloat box_limit[6]);   // Build bounding box by planes
    void set_main_buffer();                 // Initialise mesh_vbo_

public:

    Mesh();

    // Important: this method copies all the data just as it is, even
    // buffers and attributes
    Mesh(const Mesh& mesh);

    ~Mesh();

    // Reading .obj file, assuming that vertices go before normals and faces
    void load_file(const char *obj_file);
    void set_colorscheme(const ColorScheme & colorscheme);

     // !!!! Add transform matrix attr. !!!!
    void set_attributes(GLuint color_location);

    // Draw geometry
    void draw();

    // Toogle display elements
    void toogle_vertex_normals();
    void toogle_face_normals();
    void toogle_bounding_box();

};

#endif
