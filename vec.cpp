#include "vec.hpp"

//
// Two-dimensional real vector
//

// Constructors
vec2::vec2()
{
    x = 0, y = 0;
}

vec2::vec2(GLfloat s)
{
    x = s, y = s;
}

vec2::vec2(GLfloat a, GLfloat b)
{
    x = a, y = b;
    if (abs(a) < zero_tolerance) x = 0;
    if (abs(b) < zero_tolerance) y = 0;
}


// Vector normalization
void vec2::normalize(const GLfloat r)
{
    GLfloat l = sqrt(x*x + y*y);
    x /= l;
    y /= l;
}


// Inverse
vec2 vec2::operator-() const
{
    return vec2(-x, -y);
}

// Vector sum and difference
vec2 operator + (const vec2& u, const vec2& v)
{
    return vec2(u.x + v.x, u.y + v.y);
}

vec2 operator - (const vec2& u, const vec2& v)
{
    return vec2(u.x - v.x, u.y - v.y);
}


// Multiplication by a scalar
vec2 vec2::operator * (const GLfloat c) const
{
    return vec2(c * x, c * y);
}

vec2 vec2::operator / (const GLfloat c) const
{
    return vec2(x / c, y / c);
}

vec2 operator * (const GLfloat c, const vec2& v)
{
    return v * c;
}


// Compound assignment
vec2& vec2::operator += (const vec2& v)
{
    *this = vec2(x + v.x, y + v.y);
    return *this;
}

vec2& vec2::operator -= (const vec2& v)
{
    *this = vec2(x - v.x, y - v.x);
    return *this;
}

vec2& vec2::operator *= (const GLfloat c)
{
    *this = vec2(c * x, c * y);
    return *this;
}

vec2& vec2::operator /= (const GLfloat c)
{
    *this = vec2(x / c, y / c);
    return *this;
}


// Indexing operators
GLfloat& vec2::operator [] (const unsigned int i)
{
    return *(&x + i);
}

const GLfloat vec2::operator [] (const unsigned int i) const
{
    return *(&x + i);
}


// Input and output
ostream& operator << (ostream& os, const vec2& v)
{
    return os << v.x << ' ' << v.y;
}

istream& operator >> (istream& is, vec2& v)
{
    return is >> v.x >> v.y;
}


// Standard inner product
GLfloat dot(const vec2& u, const vec2& v)
{
    GLfloat val = u.x * v.x + u.y * v.y;

    if (abs(val) < zero_tolerance)
        return 0;
    else
        return val;
}


// Length of a vector
GLfloat length(const vec2& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}


// Vector normalization
vec2 normalize(const vec2& v)
{
    return vec2(v / length(v));
}



//
// Three-dimensional real vector
//

// Constructors
vec3::vec3()
{
    x = 0, y = 0, z = 0;
}

vec3::vec3(GLfloat s)
{
    x = s, y = s, z = s;
}

vec3::vec3(GLfloat a, GLfloat b, GLfloat c)
{
    x = a, y = b, z = c;
    if (abs(a) < zero_tolerance) x = 0;
    if (abs(b) < zero_tolerance) y = 0;
    if (abs(c) < zero_tolerance) z = 0;
}

vec3::vec3(const vec3& v)
{
    x = v.x, y = v.y, z = v.z;
}

vec3::vec3(const vec2& v, const GLfloat s)
{
    x = v.x, y = v.y, z = s;
}

// Vector normalization
void vec3::normalize(const GLfloat r) {
    GLfloat l = sqrt(x*x + y*y + z*z);
    x /= l;
    y /= l;
    z /= l;
}

// Inverse
vec3 vec3::operator - () const
{
    return vec3(-x, -y, -z);
}

// Vector sum and difference
vec3 operator + (const vec3& u, const vec3& v)
{
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec3 operator - (const vec3& u, const vec3& v)
{
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

// Multiplication by a scalar
vec3 vec3::operator * (const GLfloat c) const
{
    return vec3(c * x, c * y, c * z);
}

vec3 vec3::operator / (const GLfloat c) const
{
    return vec3(x / c, y / c, z / c);
}

vec3 operator * (const GLfloat c, const vec3& v)
{
    return v * c;
}

// Compound assignment
vec3& vec3::operator += (const vec3& v)
{
    *this = vec3(x + v.x, y + v.y, z + v.z);
    return *this;
}

vec3& vec3::operator -= (const vec3& v)
{
    *this = vec3(x - v.x, y - v.y, z - v.z);
    return *this;
}

vec3& vec3::operator *= (const GLfloat c)
{
    *this = vec3(x * c, y * c, z * c);
    return *this;
}

vec3& vec3::operator /= (const GLfloat c)
{
    *this = vec3(x / c, y / c, z / c);
    return *this;
}


// Indexing operators
GLfloat& vec3::operator [] (const unsigned int i)
{
    return *(&x + i);
}

const GLfloat vec3::operator [] (const unsigned int i) const
{
    return *(&x + i);
}


// Cross product
vec3 operator * (const vec3& u, const vec3& v)
{
    return vec3(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    );
}


// Input and output
ostream& operator << (ostream& os, const vec3& v)
{
    return os << v.x << ' ' << v.y << ' ' << v.z;
}

istream& operator >> (istream& is, vec3& v)
{
    return is >> v.x >> v.y >> v.z;
}


// Standard inner product
GLfloat dot(const vec3& u, const vec3& v)
{
    GLfloat val = u.x * v.x + u.y * v.y + u.z * v.z;

    if (abs(val) < zero_tolerance)
        return 0;
    else
        return val;
}

// Length of a vector
GLfloat length(const vec3& v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Vector normalization
vec3 normalize(const vec3& v) {
    return vec3(v / length(v));
}


//
// Four-dimensional vector for homogeneous coordinates
//

// Constructors
vec4::vec4()
{
    x = 0, y = 0, z = 0, w = 0;
}

vec4::vec4(const GLfloat s)
{
    x = s, y = s, z = s, w = s;
}

vec4::vec4(const vec4& v)
{
    x = v.x, y = v.y, z = v.z, w = v.w;
}

vec4::vec4(GLfloat const a, GLfloat const b, GLfloat const c, GLfloat const d)
{
    x = a, y = b, z = c, w = d;

    if (abs(a) < zero_tolerance) x = 0;
    if (abs(b) < zero_tolerance) y = 0;
    if (abs(c) < zero_tolerance) z = 0;
    if (abs(d) < zero_tolerance) w = 0;
}

vec4::vec4(const vec3& v, GLfloat s) {
    x = v.x, y = v.y, z = v.z, w = s;
}

//
// Operator overloading
//

// Indexing operators
GLfloat& vec4::operator [] (const unsigned int i)
{
    return *(&x + i);
}
const GLfloat vec4::operator [] (const unsigned int i) const
{
    return *(&x + i);
}

// Input and output operators
ostream& operator << (ostream& os, const vec4& v)
{

    return os << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
}

istream& operator >> (istream& is, vec4& v)
{
    return is >> v.x >> v.y >> v.z >> v.w;
}


//
// Math functions
//

bool belongs_to_segment(const vec2 & point, const vec2 & end_0,
    const vec2 & end_1, const double precision)
{
    if (precision <= 0)
        return false;

    double d = length(end_1 - end_0);
    unsigned int n = ceil(d / precision);
    vec2 step = (end_1 - end_0) / n;
    vec2 current;

    for (unsigned int i = 0; i < n; i++, current = end_0 + i * step)
        if (point.x <= current.x + precision &&
            point.x >= current.x - precision &&
            point.y <= current.y + precision &&
            point.y >= current.y - precision)
            return true;

    return false;
}
