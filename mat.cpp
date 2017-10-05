#include "mat.hpp"

//
// Two by two real valued matrix
//

// Constructors

mat2::mat2()
{
    M_[0] = vec2(0);
    M_[1] = vec2(0);
}

mat2::mat2(const vec2& a, const vec2& b)
{
    M_[0] = a;
    M_[1] = b;
}

mat2::mat2(const GLfloat s)
{
    M_[0] = vec2(s, 0);
    M_[1] = vec2(0, s);
}

mat2::mat2(GLfloat a00, GLfloat a01, GLfloat a10, GLfloat a11)
{
    M_[0].x = a00; M_[0].y = a01;
    M_[1].x = a10; M_[1].y = a11;
}

mat2::mat2(const mat2 & A)
{
    M_[0] = A.M_[0];
    M_[1] = A.M_[1];
}

// Operator overloading

vec2& mat2::operator [] (const unsigned int i)
{
    return M_[i];
}

const vec2& mat2::operator [] (const unsigned int i) const
{
    return M_[i];
}

mat2 mat2::operator - () const
{
    return mat2(-M_[0], -M_[1]);
}

mat2 operator + (const mat2& A, const mat2& B)
{
    return mat2(A[0] + B[0], A[1] + B[1]);
}

mat2 operator - (const mat2& A, const mat2& B)
{
    return mat2(A[0] - B[0], A[1] - B[1]);
}

mat2 mat2::operator * (const GLfloat s) const
{
    return mat2(s * M_[0], s * M_[1]);
}

mat2 mat2::operator / (const GLfloat s) const
{
    return mat2(M_[0] / s, M_[1] / s);
}

mat2 operator * (const GLfloat s, const mat2& A)
{
    return A * s;
}

mat2 operator * (const mat2& A, const mat2& B)
{
    mat2 C;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            C[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j];

    return C;
}

mat2& mat2::operator += (const mat2& A)
{
    M_[0] += A[0];
    M_[1] += A[1];
    return *this;
}

mat2& mat2::operator -= (const mat2& A)
{
    M_[0] -= A[0];
    M_[1] -= A[1];
    return *this;
}

mat2& mat2::operator *= (const GLfloat s)
{
    M_[0] *= s;
    M_[1] *= s;
    return *this;
}

mat2& mat2::operator /= (const GLfloat s)
{
    M_[0] /= s;
    M_[1] /= s;
    return *this;
}

mat2& mat2::operator *= (const mat2& A)
{
    *this = (*this) * A;
    return *this;
}

vec2 operator * (const mat2& A, const vec2& v)
{
    return vec2(dot(A[0], v), dot(A[1], v));
}

ostream& operator << (ostream& os, const mat2& A)
{
    return os << A.M_[0] << endl << A.M_[1];
}

// Some matrix functions

mat2 transpose(const mat2& A)
{
    return mat2(A[0][0], A[1][0], A[0][1], A[1][1]);
}

GLfloat det(const mat2& A)
{
    return A[0][0] * A[1][1] - A[1][0] * A[0][1];
}

mat2 Rot(const GLfloat theta)
{
    return mat2(
        cos(theta), -sin(theta),
        sin(theta),  cos(theta)
    );
}

mat2 Ref(const GLfloat theta)
{
    return mat2(
        cos(2 * theta),  sin(2 * theta),
        sin(2 * theta), -cos(2 * theta)
    );
}


//
// Three by three real valued matrix
//

// Constructors

mat3::mat3()
{
    M_[0] = vec3(0);
    M_[1] = vec3(0);
    M_[2] = vec3(0);
}

mat3::mat3(const vec3& a, const vec3& b, const vec3& c)
{
    M_[0] = a;
    M_[1] = b;
    M_[2] = c;
}

mat3::mat3(const GLfloat s)
{
    M_[0] = vec3(s, 0, 0);
    M_[1] = vec3(0, s, 0);
    M_[2] = vec3(0, 0, s);
}

mat3::mat3(
    const GLfloat a00, const GLfloat a01, const GLfloat a02,
    const GLfloat a10, const GLfloat a11, const GLfloat a12,
    const GLfloat a20, const GLfloat a21, const GLfloat a22)
{
    M_[0] = vec3(a00, a01, a02);
    M_[1] = vec3(a10, a11, a12);
    M_[2] = vec3(a20, a21, a22);
}

mat3::mat3(const mat3 & A)
{
    M_[0] = A.M_[0];
    M_[1] = A.M_[1];
    M_[2] = A.M_[2];
}

// Operator overloading

vec3& mat3::operator [] (const unsigned int i)
{
    return M_[i];
}

const vec3& mat3::operator [] (const unsigned int i) const
{
    return M_[i];
}

mat3 mat3::operator - () const
{
    return mat3(-M_[0], -M_[1], -M_[2]);
}

mat3 operator + (const mat3& A, const mat3& B)
{
    return mat3(A[0] + B[0], A[1] + B[1], A[2] + B[2]);
}

mat3 operator - (const mat3& A, const mat3& B)
{
    return mat3(A[0] - B[0], A[1] - B[1], A[2] - B[2]);
}

mat3 mat3::operator * (const GLfloat s) const
{
    return mat3(s * M_[0], s * M_[1], s * M_[2]);
}

mat3 mat3::operator / (const GLfloat s) const
{
    return mat3(M_[0] / s, M_[1] / s, M_[2] / s);
}

mat3 operator * (const GLfloat s, const mat3& A)
{
    return A * s;
}

mat3 operator * (const mat3& A, const mat3& B)
{
    mat3 C;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            C[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j];

    return C;
}

mat3& mat3::operator += (const mat3& A)
{
    M_[0] += A[0];
    M_[1] += A[1];
    M_[2] += A[2];
    return *this;
}

mat3& mat3::operator -= (const mat3& A)
{
    M_[0] -= A[0];
    M_[1] -= A[1];
    M_[2] -= A[2];
    return *this;
}

mat3& mat3::operator *= (const GLfloat s)
{
    M_[0] *= s;
    M_[1] *= s;
    M_[2] *= s;
    return *this;
}

mat3& mat3::operator /= (const GLfloat s)
{
    M_[0] /= s;
    M_[1] /= s;
    M_[2] /= s;
    return *this;
}

mat3& mat3::operator *= (const mat3& A)
{
    *this = (*this) * A;
    return *this;
}

vec3 operator * (const mat3& A, const vec3& v)
{
    return vec3(dot(A[0], v), dot(A[1], v), dot(A[2], v));
}

ostream& operator << (ostream& os, const mat3& A)
{
    return os << A[0] << endl << A[1] << endl << A[2];
}

// Some matrix functions

mat3 transpose(const mat3& A)
{
    return mat3(
        A[0][0], A[1][0], A[2][0],
        A[0][1], A[1][1], A[2][1],
        A[0][2], A[1][2], A[2][2]
    );
}

GLfloat det(const mat3& A)
{
    return
        A[0][0] * A[1][1] * A[2][2] +
        A[0][1] * A[1][2] * A[2][0] +
        A[0][2] * A[1][0] * A[2][1] -
        A[0][2] * A[1][1] * A[2][0] -
        A[0][0] * A[1][2] * A[2][1] -
        A[0][1] * A[1][0] * A[2][2];
}

// Rotations

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


//
// Homogeneous four by four matrix
//

// Constructors

mat4::mat4()
{
    M_[0] = vec4(0);
    M_[1] = vec4(0);
    M_[2] = vec4(0);
    M_[3] = vec4(0);
}

mat4::mat4(const vec4 & a, const vec4 & b, const vec4 & c, const vec4 & d)
{
    M_[0] = a;
    M_[1] = b;
    M_[2] = c;
    M_[3] = d;
}

mat4::mat4(const GLfloat s)
{
    M_[0] = vec4(s, 0, 0, 0);
    M_[1] = vec4(0, s, 0, 0);
    M_[2] = vec4(0, 0, s, 0);
    M_[3] = vec4(0, 0, 0, s);
}

mat4::mat4(
const GLfloat a00, const GLfloat a01, const GLfloat a02, const GLfloat a03,
const GLfloat a10, const GLfloat a11, const GLfloat a12, const GLfloat a13,
const GLfloat a20, const GLfloat a21, const GLfloat a22, const GLfloat a23,
const GLfloat a30, const GLfloat a31, const GLfloat a32, const GLfloat a33)
{
    M_[0] = vec4(a00, a01, a02, a03);
    M_[1] = vec4(a10, a11, a12, a13);
    M_[2] = vec4(a20, a21, a22, a23);
    M_[3] = vec4(a30, a31, a32, a33);
}


mat4::mat4(const mat4 & A)
{
    M_[0] = A.M_[0];
    M_[1] = A.M_[1];
    M_[2] = A.M_[2];
    M_[3] = A.M_[3];
}

// Operator overloading

vec4 & mat4::operator [] (const unsigned int i)
{
    return M_[i];
}

const vec4 & mat4::operator [] (const unsigned int i) const
{
    return M_[i];
}

mat4 operator * (const mat4 & A, const mat4 & B)
{
    mat4 C;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            C[i][j] =
                A[i][0] * B[0][j] +
                A[i][1] * B[1][j] +
                A[i][2] * B[2][j] +
                A[i][3] * B[3][j];

    return C;
}

ostream & operator << (ostream & os, const mat4 & A)
{
    return os << A[0] << endl << A[1] << endl << A[2] << endl << A[3];
}

mat4 RotX(const GLfloat theta)
{
    return mat4(
        1.0,        0.0,           0, 0.0,
        0.0, cos(theta), -sin(theta), 0.0,
        0.0, sin(theta),  cos(theta), 0.0,
        0.0,        0.0,         0.0, 1.0
    );
}

mat4 RotY(const GLfloat theta)
{
    return mat4(
        cos(theta), 0.0, -sin(theta), 0.0,
               0.0, 1.0,         0.0, 0.0,
        sin(theta), 0.0,  cos(theta), 0.0,
               0.0, 0.0,         0.0, 1.0
    );
}

mat4 RotZ(const GLfloat theta)
{
    return mat4(
        cos(theta), -sin(theta), 0.0, 0.0,
        sin(theta),  cos(theta), 0.0, 0.0,
               0.0,         0.0, 1.0, 0.0,
               0.0,         0.0, 0.0, 1.0
    );
}

mat4 Translate(const GLfloat x, const GLfloat y, const GLfloat z)
{
    mat4 T(1);

    T[0][3] = x;
    T[1][3] = y;
    T[2][3] = z;

    return T;
}

mat4 Translate(const vec3 & v)
{
    return Translate(v.x, v.y, v.z);
}

mat4 Translate(const vec4 & v)
{
    return Translate(v.x, v.y, v.z);
}

mat4 Scale(const GLfloat a)
{
    return mat4(
        a, 0, 0, 0,
        0, a, 0, 0,
        0, 0, a, 0,
        0, 0, 0, 1
    );
}

mat4 ScaleX(const GLfloat a)
{
    return mat4(
        a, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 ScaleY(const GLfloat a)
{
    return mat4(
        1, 0, 0, 0,
        0, a, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 ScaleZ(const GLfloat a)
{
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, a, 0,
        0, 0, 0, 1
    );
}

mat4 Scale(const GLfloat a, const GLfloat b, const GLfloat c)
{
    return mat4(
        a, 0, 0, 0,
        0, b, 0, 0,
        0, 0, c, 0,
        0, 0, 0, 1
    );
}
