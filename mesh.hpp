#include <fstream>
#include <sstream>
#include <string>
#include "list.hpp"

//
// Mesh container for .obj file
//
class Mesh {

    struct Triplet {
        unsigned int a, b, c;
        Triplet(unsigned int i = 0, unsigned int j = 0, unsigned int k = 0):
            a(i), b(j), c(k) {}
    };

//
// Geometry container: vertices, normals, faces, vertex normals corresponding
// to faces.
//
private:

    vec3 *vertecis_, *faces_, *normals_;
    int vertecis_number_, faces_number_, normals_number_;

public:

    Mesh() {
        vertecis_ = 0;
        faces_ = 0;
        normals_ = 0;
        vertecis_number_ = 0;
        faces_number_ = 0;
        normals_number_ = 0;
    }

    // Reading .obj file, assuming that vertices go before normals and faces
    Mesh(const char* obj_file) {
        vertecis_ = 0;
        normals_ = 0;
        faces_ = 0;
        vertecis_number_ = 0;
        faces_number_ = 0;
        normals_number_ = 0;

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
            else if (word == "f") {             // Faces, only triangles
                file >> A >> B >> C;
                std::stringstream X(A), Y(B), Z(C);

                Triplet f_triplet, n_triplet;

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
    }

    ~Mesh() {
        delete[] vertecis_;
        delete[] normals_;
        delete[] faces_;
    }

    vec3* vertecis() { return vertecis_; }
    int vertecis_number() { return vertecis_number_; }

    vec3* faces() { return faces_; }
    int faces_number() { return faces_number_; }

    vec3* normals() { return normals_; }
    int normals_number() { return normals_number_; }

    // Unfinished
    vec3* bounding_box() {
        vec3 max, min;

        for (int i = 0; i < vertecis_number_; i++)
            for (int j = 0; j < 3; j++)
                if (vertecis_[i][j] > max[j])
                    max[j] = vertecis_[i][j];
                else if (vertecis_[i][j] < min[j])
                    min[j] = vertecis_[i][j];

        max += vec3(0.05, 0.05, 0.05);
        min -= vec3(0.05, 0.05, 0.05);

        vec3 *box = new vec3[4];
        box[0] = vec3(min[0], min[1], min[2]);
        box[1] = vec3(min[0], max[1], min[2]);
        box[2] = vec3(min[0], max[1], max[2]);
        box[3] = vec3(max[0], max[1], max[2]);

        return box;
    }
};
