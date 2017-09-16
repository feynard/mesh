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

    enum DrawMode {WIRE, FILL};

//
// Geometry container: vertices, normals, faces, vertex normals corresponding
// to faces.
//

    vec3 *vertecis_, *faces_, *normals_;
    int vertecis_number_, faces_number_, normals_number_;

    vec4 edge_color_;

    DrawMode f_draw_mode_;          // Faces draw mode (wireframe or fill)
    bool vn_draw;                   // Draw vertex normals
    bool fn_draw;                   // Draw faces normals (need to calculate)

    GLuint buf;                     // Main vertex buffer object

public:

    Mesh() {
        vertecis_ = 0;
        faces_ = 0;
        normals_ = 0;
        vertecis_number_ = 0;
        faces_number_ = 0;
        normals_number_ = 0;
        edge_color_ = vec4(0, 0, 0, 0);
        f_draw_mode_ = WIRE;
        vn_draw = false;
        fn_draw = false;
    }

    Mesh(const char* obj_file) {
        load(obj_file);
    }

    ~Mesh() {
        delete[] vertecis_;
        delete[] normals_;
        delete[] faces_;
    }

    // Reading .obj file, assuming that vertices go before normals and faces
    void load(const char* obj_file) {
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

        // Array of vertecis
        vertecis_number_ = vertecis_list.length();
        vertecis_ = new vec3[vertecis_number_];
        for (int i = 0; i < vertecis_number_; i++)
            vertecis_[i] = vertecis_list.pop_head();

        // Array of normals
        normals_number_ = normals_list.length();
        normals_ = new vec3[normals_number_];
        for (int i = 0; i < normals_number_; i++)
            normals_[i] = normals_list.pop_head();

        // Array of faces built by vertex indeces
        faces_number_ = faces_indeces.length();
        faces_ = new vec3[faces_number_ * 3];
        for (int i = 0; i < faces_number_ * 3; i += 3) {
            Triplet t = faces_indeces.pop_head();

            faces_[i]     = vertecis_[t.a - 1];
            faces_[i + 1] = vertecis_[t.b - 1];
            faces_[i + 2] = vertecis_[t.c - 1];
        }

        glGenBuffers(1, &buf);
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glBufferData(GL_ARRAY_BUFFER, faces_number_ * 3 * sizeof(vec3), faces_,
            GL_STATIC_DRAW);
    }

    void draw() {
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, faces_number_ * 3);
    }

    vec3* vertecis() { return vertecis_; }
    int vertecis_number() { return vertecis_number_; }

    vec3* faces() { return faces_; }
    int faces_number() { return faces_number_; }

    vec3* normals() { return normals_; }
    int normals_number() { return normals_number_; }

};
