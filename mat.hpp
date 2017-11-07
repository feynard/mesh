#ifndef MAT_HPP
#define MAT_HPP

#include "graphics_root.hpp"
#include "vec.hpp"

#include <iostream>
#include <cmath>

//
// Two by two real valued matrix
//
class mat2 {

    vec2 M_[2];

public:

    //
    // Constructors
    //

    mat2();

    // Two row vectors
    mat2(const vec2& a, const vec2& b);

    // Diagonal matrix
    mat2(const GLfloat s);

    // Explicit constructor
    mat2(
    GLfloat a00, GLfloat a01,
    GLfloat a10, GLfloat a11);

    // Copy constructor
    mat2(const mat2 & A);


    //
    // Operator overloading
    //

    // Indexing operator
    vec2& operator [] (const unsigned int i);
    const vec2& operator [] (const unsigned int i) const;

    // Sum inverse
    mat2 operator - () const;

    // Sum and difference
    friend mat2 operator + (const mat2& A, const mat2& B);
    friend mat2 operator - (const mat2& A, const mat2& B);

    // Multiplication by a scalar
    mat2 operator * (const GLfloat s) const;
    mat2 operator / (const GLfloat s) const;
    friend mat2 operator * (const GLfloat c, const mat2& A);

    // Matrix multiplication
    friend mat2 operator * (const mat2& A, const mat2& B);

    // Compound assignment
    mat2& operator += (const mat2& A);
    mat2& operator -= (const mat2& A);
    mat2& operator *= (const GLfloat s);
    mat2& operator /= (const GLfloat s);
    mat2& operator *= (const mat2& A);

    // Matrix-vector multiplication
    friend vec2 operator * (const mat2& A, const vec2& v);

    // Output
    friend ostream& operator << (ostream& os, const mat2& A);
};

// Matrix operations

mat2 transpose(const mat2& A);
GLfloat det(const mat2& A);

// Anticlockwise rotation about the origin
mat2 Rot(const GLfloat theta);

// Reflection with respect to the axis that makes
// angle theta with positive x direction
mat2 Ref(const GLfloat theta);

//
// Three by three real valued matrix
//

class mat3 {

    vec3 M_[3];

public:

    //
    // Constructors
    //

    mat3();

    // Three row vectors
    mat3(const vec3& u, const vec3& v, const vec3& w);

    // Diagonal matrix
    mat3(const GLfloat s);

    // Explicit constructor
    mat3(
    const GLfloat a00, const GLfloat a01, const GLfloat a02,
    const GLfloat a10, const GLfloat a11, const GLfloat a12,
    const GLfloat a20, const GLfloat a21, const GLfloat a22);

    // Copy constructor
    mat3(const mat3 & A);

    //
    // Operator overloading
    //

    // Indexing operator
    vec3& operator [] (const unsigned int i);
    const vec3& operator [] (const unsigned int i) const;

    // Sum inverse
    mat3 operator - () const;

    // Sum and difference
    friend mat3 operator + (const mat3& A, const mat3& B);
    friend mat3 operator - (const mat3& A, const mat3& B);

    // Multiplication by a scalar
    mat3 operator * (const GLfloat s) const;
    mat3 operator / (const GLfloat s) const;
    friend mat3 operator * (const GLfloat s, const mat3& A);

    // Matrix multiplication
    friend mat3 operator * (const mat3& A, const mat3& B);

    // Compound assignments
    mat3& operator += (const mat3& A);
    mat3& operator -= (const mat3& A);
    mat3& operator *= (const GLfloat s);
    mat3& operator /= (const GLfloat s);
    mat3& operator *= (const mat3& A);

    // Matrix-vector multiplication
    friend vec3 operator * (const mat3& A, const vec3& v);

    // Output
    friend ostream& operator << (ostream& os, const mat3& A);
};

// Matrix operations
mat3 transpose(const mat3& A);
GLfloat det(const mat3& A);

// Rotations
mat3 Rx(const GLfloat theta);
mat3 Ry(const GLfloat theta);
mat3 Rz(const GLfloat theta);


//
// Homogeneous four by four matrix
//
struct mat4 {

    vec4 M_[4];

//public:

    //
    // Constructors
    //

    mat4();

    // Four row vectors
    mat4(const vec4 & a, const vec4 & b, const vec4 & c, const vec4 & d);

    // Diagonal matrix
    mat4(const GLfloat s);

    // Explicit constructor
    mat4(
    const GLfloat a00, const GLfloat a01, const GLfloat a02, const GLfloat a03,
    const GLfloat a10, const GLfloat a11, const GLfloat a12, const GLfloat a13,
    const GLfloat a20, const GLfloat a21, const GLfloat a22, const GLfloat a23,
    const GLfloat a30, const GLfloat a31, const GLfloat a32, const GLfloat a33);

    // Copy constructor
    mat4(const mat4 & A);

    //
    // Operator overloading
    //

    // Indexing operator
    vec4 & operator [] (const unsigned int i);
    const vec4 & operator [] (const unsigned int i) const;

    // Matrix multiplication
    friend mat4 operator * (const mat4 & A, const mat4 & B);

    // Matrix-vector multiplication
    friend vec4 operator * (const mat4& A, const vec4& v);
    friend vec3 operator * (const mat4& A, const vec3& v);

    // Output
    friend ostream& operator << (ostream& os, const mat4 & A);
};

// Rotation matrices generators
mat4 RotX(const GLfloat theta);
mat4 RotY(const GLfloat theta);
mat4 RotZ(const GLfloat theta);

mat4 SmallRotX(const GLfloat theta);
mat4 SmallRotY(const GLfloat theta);
mat4 SmallRotZ(const GLfloat theta);


// Scaling matrices
mat4 Scale(const GLfloat a);
mat4 ScaleX(const GLfloat a);
mat4 ScaleY(const GLfloat a);
mat4 ScaleZ(const GLfloat a);

mat4 Scale(const GLfloat a, const GLfloat b, const GLfloat c);


// Uniform translation matrix
mat4 Translate(const GLfloat x, const GLfloat y, const GLfloat z);
mat4 Translate(const vec3 & v);
mat4 Translate(const vec4 & v);

#endif
