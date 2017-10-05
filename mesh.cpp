#include "mesh.hpp"

using namespace std;

struct Triplet {
    unsigned int a, b, c;
    Triplet(unsigned int i = 0, unsigned int j = 0, unsigned int k = 0) {
        a = i, b = j, c = k;
    }
};

Mesh::Mesh()
{
    f_ = vn_ = 0;
    f_number_ = 0;
    colour_ = 0;
    set_colorscheme(solarized);
    draw_mode_[0] = draw_mode_[1] = draw_mode_[2] = false;
    mesh_vbo_ = 0;
    active = true;
    local_transform_ = 0;
    transformation = mat4(1);
    pivot = 0;
}

Mesh::Mesh(const Mesh& mesh)
{
    if (f_ == 0) {
        f_ = vn_ = 0;
        f_number_ = 0;
        colour_ = 0;
        set_colorscheme(solarized);
        draw_mode_[0] = draw_mode_[1] = draw_mode_[2] = false;
        mesh_vbo_ = 0;
        active = true;
        local_transform_ = 0;
        transformation = mat4(1);
        pivot = 0;
        return;
    }

    f_number_ = mesh.f_number_;

    for (int i = 0; i < 24; i++)
        bounding_box_[i] = mesh.bounding_box_[i];

    f_ = new vec3[f_number_ * 3];

    for (int i = 0; i < f_number_ * 3; i++)
        f_[i] = mesh.f_[i];

    if (mesh.vn_ != 0) {
        vn_ = new vec3[f_number_ * 6];
        for (int i = 0; i < f_number_ * 6; i++)
            vn_[i] = mesh.vn_[i];
    } else
        vn_ = 0;

    colour_ = mesh.colour_;
    local_transform_ = mesh.local_transform_;

    transformation = mesh.transformation;

    set_colorscheme(mesh.colorscheme_);

    for (int i = 0; i < 3; i++)
        draw_mode_[i] = mesh.draw_mode_[i];

    set_main_buffer();
}

Mesh::~Mesh()
{
    delete[] f_;
    delete[] vn_;

    if (mesh_vbo_ > 0)
        glDeleteBuffers(1, &mesh_vbo_);
}

void Mesh::set_colorscheme(const ColorScheme & colorscheme)
{
    for (int i = 0; i < 9; i++)
        colorscheme_[i] = colorscheme[i];
}

void Mesh::set_attributes(GLuint color_location, GLuint local_transform)
{
    colour_ = color_location;
    local_transform_ = local_transform;
}

void Mesh::load_file(const char* obj_file) {
    // Clear previous data
    if (f_ != 0) {
        delete[] f_;
        delete[] vn_;

        if (mesh_vbo_ != 0)
            glDeleteBuffers(1, &mesh_vbo_);
    }

    ifstream file(obj_file);
    string word, A, B, C;

    if (!file) {
        cout << "Wrong name of .obj file" << endl;
        return;
    }

    // Temporary variables used for reading .obj file

    List <vec3> vertices_list;
    List <vec3> normals_list;
    List <Triplet> faces_indeces;
    List <Triplet> normals_indeces;

    vec3 *vertices, *normals;
    unsigned int vertices_number, normals_number;

    // Assuming only triangular polygons
    while (file >> word)
        if (word[0] == '#')                 // Comment line (skip)
            getline(file, word);
        else if (word == "v")               // Vertices
            file >> vertices_list;
        else if (word == "vn")              // Vertex normals
            file >> normals_list;
        else if (word == "f") {             // Faces (only triangles)
            Triplet f_triplet, n_triplet;

            file >> A >> B >> C;
            stringstream X(A), Y(B), Z(C);
            X >> f_triplet.a, Y >> f_triplet.b, Z >> f_triplet.c;

            // Indeces of vertices of a current face
            faces_indeces.push(f_triplet);

            if (X.peek() == '/' && Y.peek() == '/' && Z.peek() == '/') {
                X.get(), Y.get(), Z.get();

                if (X.peek() == '/' && Y.peek() == '/' && Z.peek() == '/')
                    X.get(), Y.get(), Z.get();

                X >> n_triplet.a, Y >> n_triplet.b, Z >> n_triplet.c;

                // Indeces of normals of a current face's vertices
                normals_indeces.push(n_triplet);
            }
        }

    // x_min, x_max, y_min, y_max, z_min, z_max
    GLfloat box_limit[6];
    for (int i = 0; i < 3; i++) {
        box_limit[2 * i] = vertices_list[0][i];
        box_limit[2 * i + 1] = vertices_list[0][i];
    }

    // Array of vertices
    vertices_number = vertices_list.length();
    vertices = new vec3[vertices_number];
    for (unsigned int i = 0; i < vertices_number; i++) {
        vertices[i] = vertices_list.pop_head();

        // Center of a model
        pivot += vertices[i];

        // Check bounding box limits
        for (int j = 0; j < 3; j++) {
            if (vertices[i][j] < box_limit[2 * j])
                box_limit[2 * j] = vertices[i][j];
            if (vertices[i][j] > box_limit[2 * j + 1])
                box_limit[2 * j + 1] = vertices[i][j];
        }
    }

    // Calculating center of a model
    pivot /= vertices_number;

    build_box(box_limit);

    // Array of vertex normals
    normals_number = normals_list.length();
    normals = new vec3[normals_number];
    for (unsigned int i = 0; i < normals_number; i++)
        normals[i] = normals_list.pop_head();

    f_number_ = faces_indeces.length();
    f_ = new vec3[f_number_ * 3];

    if (normals_number != 0)
        vn_ = new vec3[f_number_ * 6];

    for (int i = 0; i < f_number_ * 3; i += 3) {

        Triplet t = faces_indeces.pop_head();

        // Filling faces array
        f_[i]     = vertices[t.a - 1];
        f_[i + 1] = vertices[t.b - 1];
        f_[i + 2] = vertices[t.c - 1];

        // Filling vertex normals array
        if (normals_number != 0) {

            Triplet y = normals_indeces.pop_head();

            vn_[2 * i]     = f_[i];
            vn_[2 * i + 1] = f_[i]     + normals[y.a - 1] / 20;
            vn_[2 * i + 2] = f_[i + 1];
            vn_[2 * i + 3] = f_[i + 1] + normals[y.b - 1] / 20;
            vn_[2 * i + 4] = f_[i + 2];
            vn_[2 * i + 5] = f_[i + 2] + normals[y.c - 1] / 20;
        }
    }

    set_main_buffer();

    delete[] vertices;
    delete[] normals;
}

void Mesh::set_main_buffer()
{
    // Create buffer
    glGenBuffers(1, &mesh_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_vbo_);

    // Put vertex normals in buffer if they exist
    if (vn_ != 0) {
        glBufferData(GL_ARRAY_BUFFER,
            (f_number_ * 9 + 24) * sizeof(vec3), f_, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, (f_number_ * 3 + 24) * sizeof(vec3),
            f_number_ * 6 * sizeof(vec3), vn_);
    } else {
        glBufferData(GL_ARRAY_BUFFER, (f_number_ * 3 + 24) * sizeof(vec3),
            f_, GL_STATIC_DRAW);
    }

    // Bounding box
    glBufferSubData(GL_ARRAY_BUFFER, f_number_ * 3 * sizeof(vec3),
        24 * sizeof(vec3), bounding_box_);
}

void Mesh::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, mesh_vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    mat4 id(1);

    // Drawing model

    glUniformMatrix4fv(local_transform_, 1, true ,(GLfloat*) & transformation);

    if (!active) {
        draw_mode_[0] = false, draw_mode_[1] = false, draw_mode_[2] = false;

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform4fv(colour_, 1, (GLfloat*) & colorscheme_[8]);
        glDrawArrays(GL_TRIANGLES, 0, f_number_ * 3);
        glUniformMatrix4fv(local_transform_, 1, true ,(GLfloat*) & id);
        return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform4fv(colour_, 1, (GLfloat*) & colorscheme_[3]);
    glDrawArrays(GL_TRIANGLES, 0, f_number_ * 3);

    // Drawing vertex normals
    if (vn_ != 0 && draw_mode_[0] == true) {
        glUniform4fv(colour_, 1, (GLfloat*) & colorscheme_[1]);
        glDrawArrays(GL_LINES, f_number_ * 3 + 24, f_number_ * 6);
    }

    // Drawing bounding box in model coordinates
    if (draw_mode_[2]) {
        glUniform4fv(colour_, 1, (GLfloat*) &colorscheme_[7]);
        glDrawArrays(GL_LINES, f_number_ * 3, 24);
    }

    glUniformMatrix4fv(local_transform_, 1, true ,(GLfloat*) & id);
}


void Mesh::toogle_vertex_normals()
{
    draw_mode_[0] ? draw_mode_[0] = false : draw_mode_[0] = true;
}

void Mesh::toogle_face_normals()
{
    draw_mode_[1] ? draw_mode_[1] = false : draw_mode_[1] = true;
}

void Mesh::toogle_bounding_box()
{
    draw_mode_[2] ? draw_mode_[2] = false : draw_mode_[2] = true;
}

void Mesh::build_box(GLfloat box_limit[6]) {
    GLfloat x_min = box_limit[0] - 0.05, x_max = box_limit[1] + 0.05,
            y_min = box_limit[2] - 0.05, y_max = box_limit[3] + 0.05,
            z_min = box_limit[4] - 0.05, z_max = box_limit[5] + 0.05;

    bounding_box_[0]  = vec3(x_min, y_min, z_min);
    bounding_box_[1]  = vec3(x_max, y_min, z_min);
    bounding_box_[2]  = vec3(x_min, y_min, z_max);
    bounding_box_[3]  = vec3(x_max, y_min, z_max);
    bounding_box_[4]  = vec3(x_min, y_max, z_min);
    bounding_box_[5]  = vec3(x_max, y_max, z_min);
    bounding_box_[6]  = vec3(x_min, y_max, z_max);
    bounding_box_[7]  = vec3(x_max, y_max, z_max);
    bounding_box_[8]  = vec3(x_min, y_min, z_min);
    bounding_box_[9]  = vec3(x_min, y_max, z_min);
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
}
