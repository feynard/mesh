#ifndef MESH_HPP
#define MESH_HPP

#include <fstream>
#include <sstream>
#include <string>

#include "vec.hpp"
#include "mat.hpp"
#include "list.hpp"

#include "graphics.hpp"

//
// Mesh container
//
class Mesh {

    struct Triplet {
        unsigned int a, b, c;
        Triplet(unsigned int i = 0, unsigned int j = 0, unsigned int k = 0) {
            a = i, b = j, c = k;
        }
    };

    // Geometric data
    vec3 *vertecis_, *faces_, *normals_, *vertex_normals_, *faces_normals_;
    int vertecis_number_, faces_number_, normals_number_;
    int *faces_mid_indices_;

    vec3 center_;
    vec3 bounding_box_[24];

    GLuint color_loc_;              // Color attribute location
    vec4 face_color_;
    vec4 vertex_normal_color_;
    vec4 face_normal_color_;
    vec4 box_color_;

    bool box_draw_;
    bool vn_draw_;                  // Draw vertex normals
    bool fn_draw_;                  // Draw faces normals (need to calculate)

    GLuint buf_;                     // Main vertex buffer object

    // Build bounding box
    void build_box(GLfloat box_limit[6]);

public:
    Mesh();
    Mesh(const Mesh& mesh);
    Mesh(const char* obj_file);
    ~Mesh();

    // Reading .obj file, assuming that vertices go before normals and faces
    void load(const char* obj_file);

    // Draw geometry
    void draw();

    // Set color attribute in fragment shader
    void set_color_attribute(GLuint color_location);

    // Toogle display
    void toogle_vertex_normals();
    void toogle_face_normals();
    void toogle_bounding_box();

    // Getters
    vec3* vertecis();
    int vertecis_number();

    vec3* faces();
    int faces_number();

    vec3* normals();
    int normals_number();

};

#endif
