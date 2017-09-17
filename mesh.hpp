#include <fstream>
#include <sstream>
#include <string>
#include "list.hpp"

//
// Mesh container
//
class Mesh {

    struct Triplet {
        unsigned int a, b, c;
        Triplet(unsigned int i = 0, unsigned int j = 0, unsigned int k = 0):
            a(i), b(j), c(k) {}
    };

    // Geometric data
    vec3 *vertecis_, *faces_, *normals_, *vertex_normals_, *faces_normals_;
    int vertecis_number_, faces_number_, normals_number_;
    int *faces_mid_indices_;

    vec3 bounding_box_[24];

    GLuint color_loc_;              // Color attribute location
    vec4 face_color_;
    vec4 vertex_normal_color_;
    vec4 face_normal_color_;
    vec4 box_color_;

    bool box_draw_;
    bool vn_draw_;                  // Draw vertex normals
    bool fn_draw_;                  // Draw faces normals (need to calculate)

    GLuint buf;                     // Main vertex buffer object

public:

    Mesh() {
        vertecis_ = 0;
        faces_ = 0;
        normals_ = 0;
        vertex_normals_ = 0;
        faces_normals_ = 0;
        vertecis_number_ = 0;
        faces_number_ = 0;
        normals_number_ = 0;
        faces_mid_indices_ = 0;
        face_color_ = vec4(220 / 255.0, 50 / 255.0, 47 / 255.0, 1);
        vertex_normal_color_ = vec4(181 / 255.0, 137 / 255.0, 0 / 255.0, 1);
        face_normal_color_ = vec4(211 / 255.0, 54 / 255.0, 130 / 255.0, 1);
        box_color_ = vec4(88 / 255.0, 110 / 255.0, 117 / 255.0, 1);
        vn_draw_ = false;
        fn_draw_ = false;
        box_draw_ = false;
        buf = 0;
    }

    Mesh(const char* obj_file) {
        load(obj_file);
    }

    ~Mesh() {
        delete[] vertecis_;
        delete[] normals_;
        delete[] faces_;
        delete[] vertex_normals_;
        delete[] faces_normals_;
        delete[] faces_mid_indices_;

        if (buf != 0)
            glDeleteBuffers(1, &buf);
    }

    // Reading .obj file, assuming that vertices go before normals and faces
    void load(const char* obj_file) {

        // Clear previous data
        if (vertecis_ != 0) {
            delete[] vertecis_;
            delete[] normals_;
            delete[] faces_;
            delete[] vertex_normals_;
            delete[] faces_normals_;
            delete[] faces_mid_indices_;

            if (buf != 0)
                glDeleteBuffers(1, &buf);
        }

        std::ifstream file(obj_file);
        std::string word, A, B, C;

        if (!file) {
            std::cout << "Wrong name of .obj file" << std::endl;
            return;
        }

        List <vec3> vertecis_list;
        List <vec3> normals_list;
        List <Triplet> faces_indeces;
        List <Triplet> normals_indeces;

        //
        // Assuming only triangular polygons
        //
        while (file >> word)
            if (word[0] == '#')                 // Comment line
                getline(file, word);
            else if (word == "v")               // vertecis
                file >> vertecis_list;
            else if (word == "vn")              // Vertex normals
                file >> normals_list;
            else if (word == "f") {             // Faces (only triangles)
                Triplet f_triplet, n_triplet;

                file >> A >> B >> C;
                std::stringstream X(A), Y(B), Z(C);
                X >> f_triplet.a, Y >> f_triplet.b, Z >> f_triplet.c;

                faces_indeces.push(f_triplet);

                if (X.peek() == '/' && Y.peek() == '/' && Z.peek() == '/') {
                    X.get(), Y.get(), Z.get();

                    if (X.peek() == '/' && Y.peek() == '/' && Z.peek() == '/')
                        X.get(), Y.get(), Z.get();

                    X >> n_triplet.a, Y >> n_triplet.b, Z >> n_triplet.c;

                    normals_indeces.push(n_triplet);
                }
            }


        // x_min, x_max, y_min, y_max, z_min, z_max
        GLfloat box_limit[6];
        for (int i = 0; i < 3; i++)
            box_limit[2 * i + 1] = vertecis_list[0][i];

        // Array of vertecis
        vertecis_number_ = vertecis_list.length();
        vertecis_ = new vec3[vertecis_number_];
        for (int i = 0; i < vertecis_number_; i++) {
            vertecis_[i] = vertecis_list.pop_head();

            // Box limits
            for (int j = 0; j < 3; j++) {
                if (vertecis_[i][j] < box_limit[2 * j])
                    box_limit[2 * j] = vertecis_[i][j];
                if (vertecis_[i][j] > box_limit[2 * j + 1])
                    box_limit[2 * j + 1] = vertecis_[i][j];
            }
        }

        // Create bounding box (in model frame)

        GLfloat x_min = box_limit[0] - 0.1, x_max = box_limit[1] + 0.1,
                y_min = box_limit[2] - 0.1, y_max = box_limit[3] + 0.1,
                z_min = box_limit[4] - 0.1, z_max = box_limit[5] + 0.1;

        bounding_box_[0] = vec3(x_min, y_min, z_min);
        bounding_box_[1] = vec3(x_max, y_min, z_min);
        bounding_box_[2] = vec3(x_min, y_min, z_max);
        bounding_box_[3] = vec3(x_max, y_min, z_max);
        bounding_box_[4] = vec3(x_min, y_max, z_min);
        bounding_box_[5] = vec3(x_max, y_max, z_min);
        bounding_box_[6] = vec3(x_min, y_max, z_max);
        bounding_box_[7] = vec3(x_max, y_max, z_max);
        bounding_box_[8] = vec3(x_min, y_min, z_min);
        bounding_box_[9] = vec3(x_min, y_max, z_min);
        bounding_box_[10] = vec3(x_min, y_min, z_max);
        bounding_box_[11] = vec3(x_min, y_max, z_max);
        bounding_box_[12] = vec3(x_max, y_min, z_min);
        bounding_box_[13] = vec3(x_max, y_max, z_min);
        bounding_box_[14] = vec3(x_max, y_min, z_max);
        bounding_box_[15] = vec3(x_max, y_max, z_max);
        bounding_box_[16] = vec3(x_min, y_min, z_min);
        bounding_box_[17] = vec3(x_min, y_min, z_max);
        bounding_box_[18] = vec3(x_min, y_max, z_min);
        bounding_box_[19] = vec3(x_min, y_max, z_max);
        bounding_box_[20] = vec3(x_max, y_min, z_min);
        bounding_box_[21] = vec3(x_max, y_min, z_max);
        bounding_box_[22] = vec3(x_max, y_max, z_min);
        bounding_box_[23] = vec3(x_max, y_max, z_max);


        // Array of vertex normals
        normals_number_ = normals_list.length();
        normals_ = new vec3[normals_number_];
        for (int i = 0; i < normals_number_; i++)
            normals_[i] = normals_list.pop_head();


        // Array of faces and vertex normals built indeces
        // Number of points in vertex_normals_ is equal to 6 * faces_number_

        faces_number_ = faces_indeces.length();

        faces_ = new vec3[faces_number_ * 3];

        if (normals_number_ != 0) {
            vertex_normals_ = new vec3[faces_number_ * 6];
            faces_normals_ = new vec3[faces_number_ * 2];
        }

        for (int i = 0; i < faces_number_ * 3; i += 3) {
            Triplet t = faces_indeces.pop_head();

            // Filling faces array
            faces_[i]     = vertecis_[t.a - 1];
            faces_[i + 1] = vertecis_[t.b - 1];
            faces_[i + 2] = vertecis_[t.c - 1];

            // Filling vertex and face normals arrays if they exist
            if (normals_number_ != 0) {

                Triplet y = normals_indeces.pop_head();

                vertex_normals_[2*i]     = faces_[i];
                vertex_normals_[2*i + 1] = faces_[i] +
                    normals_[y.a - 1] / 10;

                vertex_normals_[2*i + 2] = faces_[i + 1];
                vertex_normals_[2*i + 3] = faces_[i + 1] +
                    normals_[y.b - 1] / 10;

                vertex_normals_[2*i + 4] = faces_[i + 2];
                vertex_normals_[2*i + 5] = faces_[i + 2] +
                    normals_[y.c - 1] / 10;


                vec3 face_normal, mid_point;
                face_normal = (
                    normals_[y.a - 1] +
                    normals_[y.b - 1] +
                    normals_[y.c - 1]) / 30;

                mid_point = (
                    vertecis_[t.a - 1] +
                    vertecis_[t.b - 1] +
                    vertecis_[t.c - 1]) / 3;

                faces_normals_[i / 3 * 2] = mid_point;
                faces_normals_[i / 3 * 2 + 1] = mid_point + face_normal;
            }
        }

        // Create buffer
        glGenBuffers(1, &buf);
        glBindBuffer(GL_ARRAY_BUFFER, buf);

        // Put vertex normals in buffer if they exist
        if (normals_number_ != 0) {
            glBufferData(GL_ARRAY_BUFFER, faces_number_ * 11 * sizeof(vec3) + 24 * sizeof(vec3),
                faces_, GL_STATIC_DRAW);
        //    glBufferSubData(GL_ARRAY_BUFFER, 0,
        //        faces_number_ * 3 * sizeof(vec3), faces_);
            glBufferSubData(GL_ARRAY_BUFFER, faces_number_ * 3 * sizeof(vec3),
                faces_number_ * 6 * sizeof(vec3), vertex_normals_);
            glBufferSubData(GL_ARRAY_BUFFER, faces_number_ * 9 * sizeof(vec3),
                faces_number_ * 2 * sizeof(vec3), faces_normals_);
            glBufferSubData(GL_ARRAY_BUFFER, faces_number_ * 11 * sizeof(vec3),
                24 * sizeof(vec3), bounding_box_);



            faces_mid_indices_ = new int[faces_number_];
            for (int i = 0; i < faces_number_; i++)
                faces_mid_indices_[i] = faces_number_ * 9 + 2 * i;

        } else {
            glBufferData(GL_ARRAY_BUFFER, faces_number_ * 3 * sizeof(vec3) + 24 * sizeof(vec3),
                faces_, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, faces_number_ * 3 * sizeof(vec3),
                24 * sizeof(vec3), bounding_box_);
        }

    }

    void draw() {
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Drawing model
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform4fv(color_loc_, 1, (GLfloat*) &face_color_);
        glDrawArrays(GL_TRIANGLES, 0, faces_number_ * 3);

        // Drawing vertex normals
        if (normals_number_ != 0 && vn_draw_ == true) {
            glUniform4fv(color_loc_, 1, (GLfloat*) &vertex_normal_color_);
            glDrawArrays(GL_LINES, faces_number_ * 3, faces_number_ * 6);
        }

        // Drawing face normals
        if (normals_number_ != 0 && fn_draw_ == true) {
            glUniform4fv(color_loc_, 1, (GLfloat*) &face_normal_color_);
            glDrawArrays(GL_LINES, faces_number_ * 9, faces_number_ * 2);

            glPointSize(3);
            glDrawElements(GL_POINTS, faces_number_, GL_UNSIGNED_INT, faces_mid_indices_);
        }

        // Drawing bounding box in model coordinates
        if (box_draw_) {
            glUniform4fv(color_loc_, 1, (GLfloat*) &box_color_);
            glDrawArrays(GL_LINES, faces_number_ * 11, 24);
        }
    }

    void color(GLuint color_location) { color_loc_ = color_location; }

    void vertex_normals_drawing()
    { vn_draw_ ? vn_draw_ = false : vn_draw_ = true; }

    void face_normals_drawing()
    { fn_draw_ ? fn_draw_ = false : fn_draw_ = true; }

    void box_drawing()
    { box_draw_ ? box_draw_ = false : box_draw_ = true; }

    vec3* vertecis() { return vertecis_; }
    int vertecis_number() { return vertecis_number_; }

    vec3* faces() { return faces_; }
    int faces_number() { return faces_number_; }

    vec3* normals() { return normals_; }
    int normals_number() { return normals_number_; }

};
