#ifndef VEC_HPP
#define VEC_HPP

#include <OpenGL/OpenGL.h>

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Zero tolerance, that is any number less than this considered to be 0
const GLfloat zero_tolerance = 1e-7;

//
// Two-dimensional real vector
//
struct vec2 {
    GLfloat x;
    GLfloat y;

    // Constructors
    vec2();
    vec2(GLfloat s) {x = s, y = s;}
    vec2(const GLfloat a, const GLfloat b);
    vec2(const vec2& v);

    // Vector normalization to a given value
    void normalize(const GLfloat r);

    //
    // Operator overloading
    //

    // Inverse
    vec2 operator - () const;

    // Vector sum and difference
    friend vec2 operator + (const vec2& u, const vec2& v);
    friend vec2 operator - (const vec2& u, const vec2& v);

    // Multiplication by a scalar
    vec2 operator * (const GLfloat c) const;
    vec2 operator / (const GLfloat c) const;
    friend vec2 operator * (const GLfloat c, const vec2& v);

    // Compound assignment
    vec2& operator += (const vec2& v);
    vec2& operator -= (const vec2& v);
    vec2& operator *= (const GLfloat c);
    vec2& operator /= (const GLfloat c);

    // Indexing operators
    GLfloat& operator [] (const unsigned int i);
    const GLfloat operator [] (const unsigned int i) const;

    // Input and output
    friend ostream& operator << (ostream& os, const vec2& v);
    friend istream& operator >> (istream& is, vec2& v);
};

// Standard inner product
GLfloat dot(const vec2& u, const vec2& v);

// Length of a vector
GLfloat length(const vec2& v);

// Normalized vector
vec2 normalize(const vec2& v);


//
// Three-dimensional real vector
//
struct vec3 {
    GLfloat x;
    GLfloat y;
    GLfloat z;

    // Constructors
    vec3();
    vec3(GLfloat s) {x = s, y = s, z = s; }
    vec3(const GLfloat a, const GLfloat b, const GLfloat c);
    vec3(const vec3& v);
    vec3(const vec2& v);
    vec3(const vec2& v, const GLfloat s);


    // Vector normalization
    void normalize();
    void normalize(const GLfloat r);

    //
    // Operator overloading
    //

    // Inverse
    vec3 operator - () const;

    // Vector sum and difference
    friend vec3 operator + (const vec3& u, const vec3& v);
    friend vec3 operator - (const vec3& u, const vec3& v);

    // Multiplication by a scalar
    vec3 operator * (const GLfloat c) const;
    vec3 operator / (const GLfloat c) const;
    friend vec3 operator * (const GLfloat c, const vec3& v);

    // Compound assignment
    vec3& operator += (const vec3& v);
    vec3& operator -= (const vec3& v);
    vec3& operator *= (const GLfloat c);
    vec3& operator /= (const GLfloat c);

    // Indexing operators
    GLfloat& operator [] (const unsigned int i);
    const GLfloat operator [] (const unsigned int i) const;

    // Cross product
    friend vec3 operator * (const vec3& u, const vec3& v);

    // Input and output
    friend ostream& operator << (ostream& os, const vec3& v);
    friend istream& operator >> (istream& is, vec3& v);
};

// Standard inner product
GLfloat dot(const vec3& u, const vec3& v);

// Length of a vector
GLfloat length(const vec3& v);

// Normalized vector
vec3 normalize(const vec3& v);


//
// Four-dimensional vector for homogeneous coordinates
//
struct vec4 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;

    // Constructors
    vec4();
    vec4(const GLfloat a, const GLfloat b, const GLfloat c, const GLfloat d);
    vec4(const vec4& v);
    vec4(const vec3& v);
    vec4(const vec3& v, const GLfloat s);

    //
    // Operator overloading
    //

    // Indexing operators
    GLfloat& operator [] (const unsigned int i);
    const GLfloat operator [] (const unsigned int i) const;

    // Extraction operator
    friend ostream& operator << (ostream& os, const vec4& v);
    friend istream& operator >> (istream& is, vec4& v);
};

#endif
