#include "vec.hpp"

//
// Two by two real matrix
//
class mat2 {

    vec2 M[2];

public:

    //
    // Constructors
    //

    // Two row vectors
    mat2(const vec2& u, const vec2& v)
    { M[0] = u; M[1] = v; }

    // Diagonal matrix
    mat2(const GLfloat c = 1)
    { M[0].x = c;  M[1].y = c; }

    // Explicit constructor
    mat2(GLfloat a00, GLfloat a01, GLfloat a10, GLfloat a11)
    { M[0].x = a00; M[0].y = a01; M[1].x = a10; M[1].y = a11; }


    //
    // Operator overloading
    //

    // Indexing operator
    vec2& operator [] (int i) { return M[i]; }
    const vec2& operator [] (int i) const { return M[i]; }

    // Sum inverse
    mat2 operator - () const
    { return mat2(-M[0], -M[1]); }

    // Sum and difference
    friend mat2 operator + (const mat2& A, const mat2& B)
    { return mat2(A[0] + B[0], A[1] + B[1]); }

    friend mat2 operator - (const mat2& A, const mat2& B)
    { return mat2(A[0] - B[0], A[1] - B[1]); }

    // Multiplication by a scalar
    mat2 operator * (const GLfloat c) const { return mat2(c*M[0], c*M[1]); }
    mat2 operator / (const GLfloat c) const { return mat2(M[0]/c, M[1]/c); }
    friend mat2 operator * (const GLfloat c, const mat2& A) { return A * c; }

    // Matrix multiplication
    friend mat2 operator * (const mat2& A, const mat2& B) {
        return mat2(
            A[0][0] * B[0][0] + A[0][1] * B[1][0],
            A[0][0] * B[0][1] + A[0][1] * B[1][1],
            A[1][0] * B[0][0] + A[1][1] * B[1][0],
            A[1][0] * B[0][1] + A[1][1] * B[1][1]
        );
    }

    // Compound assignment
    mat2& operator += (const mat2& A)
    { M[0] += A[0]; M[1] += A[1]; return *this; }

    mat2& operator -= (const mat2& A)
    { M[0] -= A[0]; M[1] -= A[1]; return *this; }

    mat2& operator *= (const GLfloat c)
    { M[0] *= c; M[1] *= c; return *this; }

    mat2& operator /= (const GLfloat c)
    { M[0] /= c; M[1] /= c; return *this; }

    mat2& operator *= (const mat2& A)
    { *this = (*this) * A; return *this; }

    // Matrix-vector operations
    //
    friend vec2 operator * (const mat2& A, const vec2& v)
    { return vec2(dot(A[0], v), dot(A[1], v)); }
};

// Transpose
mat2 transpose(const mat2& A)
{
    return mat2(A[0][0], A[1][0], A[0][1], A[1][1]);
}

// Determinant
GLfloat det(const mat2& A)
{
    return A[0][0] * A[1][1] - A[1][0] * A[0][1];
}

// Rotation by an angle theta
mat2 Rot(const GLfloat theta)
{
    return mat2(
        cos(theta), -sin(theta),
        sin(theta),  cos(theta)
    );
}

// Reflection about the axis that forms angle theta with x-axis
mat2 Ref(const GLfloat theta)
{
    return mat2(
        cos(2 * theta),  sin(2 * theta),
        sin(2 * theta), -cos(2 * theta)
    );
}

class mat3 {

    vec3 M[3];

public:

    mat3(
        GLfloat a00, GLfloat a01, GLfloat a02,
        GLfloat a10, GLfloat a11, GLfloat a12,
        GLfloat a20, GLfloat a21, GLfloat a22) {

        M[0].x = a00, M[0].y = a01, M[0].z = a02,
        M[1].x = a10, M[1].y = a11, M[1].z = a12,
        M[2].x = a20, M[2].y = a21, M[2].z = a22;

        }

    vec3& operator [] (int i) { return M[i]; }
    const vec3& operator [] (int i) const { return M[i]; }


    friend mat3 operator * (const mat3& A, const mat3& B) {
        return mat3(
            A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0],
            A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1],
            A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2],

            A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0],
            A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1],
            A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2],

            A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0],
            A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1],
            A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2]
        );
    }

    friend vec3 operator * (const mat3& A, const vec3& v)
    { return vec3(dot(A[0], v), dot(A[1], v), dot(A[2], v)); }
};

mat3 Rx(const GLfloat theta)
{
    return mat3(
        1.0,        0.0,           0,
        0.0, cos(theta), -sin(theta),
        0.0, sin(theta),  cos(theta)
    );
}

mat3 Ry(const GLfloat theta)
{
    return mat3(
        cos(theta), 0.0, -sin(theta),
               0.0, 1.0,         0.0,
        sin(theta), 0.0,  cos(theta)
    );
}

mat3 Rz(const GLfloat theta)
{
    return mat3(
        cos(theta), -sin(theta), 0.0,
        sin(theta),  cos(theta), 0.0,
               0.0,         0.0, 1.0
    );
}
