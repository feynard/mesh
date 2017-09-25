#ifndef MAT_HPP
#define MAT_HPP

#include <OpenGL/OpenGL.h>
#include <iostream>
#include <cmath>

#include "vec.hpp"

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
    mat2(const vec2& u, const vec2& v);

    // Diagonal matrix
    mat2(const GLfloat c);

    // Explicit constructor
    mat2(GLfloat a00, GLfloat a01, GLfloat a10, GLfloat a11);

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
    mat3(const GLfloat c);

    // Explicit constructor
    mat3(
        const GLfloat a00, const GLfloat a01, const GLfloat a02,
        const GLfloat a10, const GLfloat a11, const GLfloat a12,
        const GLfloat a20, const GLfloat a21, const GLfloat a22);

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

#endif
