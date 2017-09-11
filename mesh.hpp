#include <fstream>
#include <sstream>
#include <string>
#include "list.hpp"

//
// Mesh container for .obj file
//
class Mesh {

private:

    struct Triplet {
        unsigned int a, b, c;
        Triplet(unsigned int i = 0, unsigned int j = 0, unsigned int k = 0):
            a(i), b(j), c(k) {}
    };

    int verteces_number_, faces_number_, normals_number_;
    vec3 *verteces_, *faces_, *normals_;

public:

    Mesh() {
        verteces_number_ = 0;
        faces_number_ = 0;
        normals_number_ = 0;
        verteces_ = 0;
        faces_ = 0;
        normals_ = 0;
    }

    // Reading .obj file, assuming that vertices go before normals and faces
    Mesh(const char* obj_file) {
        verteces_number_ = 0;
        faces_number_ = 0;
        normals_number_ = 0;
        verteces_ = 0;
        normals_ = 0;
        faces_ = 0;

        std::ifstream file(obj_file);
        std::string word, A, B, C;

        if (!file) {
            std::cout << "Wrong name of .obj file" << std::endl;
            return;
        }

        List <vec3> verteces_list;
        //List <vec3> normals_list;
        List <Triplet> faces_indeces;
        //List <Triplet> normals_indeces;

        //
        // Here assumed that there are two '/' between the face and normal
        // indeces, i.e. 1//2
        //
        while (file >> word)
            if (word[0] == '#')                 // Comment line
                getline(file, word);
            else if (word == "v")               // Verteces
                file >> verteces_list;
//            else if (word == "vn")              // Vertex normals
//                file >> normals_list;
            else if (word == "f") {             // Faces, only triangles
                file >> A >> B >> C;
                std::stringstream X(A), Y(B), Z(C);

                Triplet face_triplet;//, normal_triplet;

                X >> face_triplet.a;
                Y >> face_triplet.b;
                Z >> face_triplet.c;

                faces_indeces.push(face_triplet);
/*
                if (X.peek() == '/' && Y.peek() == '/' && Z.peek() == '/') {
                    X.get(), Y.get(), Z.get();

                    if (X.peek() == '/' && Y.peek() == '/' && Z.peek() == '/')
                        X.get(), Y.get(), Z.get();

                    X >> normal_triplet.a;
                    Y >> normal_triplet.b;
                    Z >> normal_triplet.c;

                    normals_indeces.push(face_triplet);
                } else
                    continue;
*/
            }

        verteces_number_ = verteces_list.length();
        verteces_ = new vec3[verteces_number_];
        for (int i = 0; i < verteces_number_; i++)
            verteces_[i] = verteces_list.pop_head();

        faces_number_ = faces_indeces.length();
        faces_ = new vec3[faces_number_ * 3];
        for (int i = 0; i < faces_number_ * 3; i += 3) {
            Triplet t = faces_indeces.pop_head();

            faces_[i]     = verteces_[t.a - 1];
            faces_[i + 1] = verteces_[t.b - 1];
            faces_[i + 2] = verteces_[t.c - 1];
        }
    }

    ~Mesh() {
        delete[] verteces_;
        delete[] normals_;
        delete[] faces_;
    }

    vec3* verteces() { return verteces_; }
    int verteces_number() { return verteces_number_; }

    vec3* faces() { return faces_; }
    int faces_number() { return faces_number_; }

    vec3* normals() { return normals_; }
    int normals_number() { return normals_number_; }

    // Unfinished
    vec3* bounding_box() {
        vec3 max, min;

        for (int i = 0; i < verteces_number_; i++)
            for (int j = 0; j < 3; j++)
                if (verteces_[i][j] > max[j])
                    max[j] = verteces_[i][j];
                else if (verteces_[i][j] < min[j])
                    min[j] = verteces_[i][j];

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
