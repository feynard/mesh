// Zero tolerance, that is any number less than this considered to be 0
const GLfloat zero_tolerance = 1e-7;


//
// Two-dimensional real vector
//
struct vec2 {
    GLfloat x;
    GLfloat y;

    // Constructor
    vec2 (GLfloat a = 0, GLfloat b = 0) {
        x = a, y = b;
        if (std::abs(a) < zero_tolerance) x = 0;
        if (std::abs(b) < zero_tolerance) y = 0;
    }

    // Vector normalization
    void normalize() {
        double l = sqrt(x*x + y*y);
        x /= l;
        y /= l;
    }

    void normalize(double r) {
        double l = sqrt(x*x + y*y);
        x = x / l * r;
        y = y / l * r;
    }

    //
    // Operator overloading
    //

    // Inverse
    vec2 operator - () const
    { return vec2(-x, -y); }

    // Vector sum and difference
    friend vec2 operator + (const vec2& u, const vec2& v)
    { return vec2(u.x + v.x, u.y + v.y); }

    friend vec2 operator - (const vec2& u, const vec2& v)
    { return vec2(u.x - v.x, u.y - v.y); }

    // Multiplication by a scalar
    vec2 operator * (const GLfloat c) const { return vec2(c*x, c*y); }
    vec2 operator / (const GLfloat c) const { return vec2(x/c, y/c); }
    friend vec2 operator * (const GLfloat c, const vec2& v) { return v * c; }

    // Compound assignment
    vec2& operator += (const vec2& v)
    { *this = vec2(x + v.x, y + v.y); return *this; }

    vec2& operator -= (const vec2& v)
    { *this = vec2(x - v.x, y - v.x); return *this; }

    vec2& operator *= (const GLfloat c)
    { *this = vec2(c*x, c*y); return *this; }

    vec2& operator /= (const GLfloat c)
    { *this = vec2(x/c, y/c); return *this; }

    // Indexing operators
    GLfloat& operator [] (int i) { return *(&x + i); }
    const GLfloat operator [] (int i) const { return *(&x + i); }

    // Extraction operator
    friend std::ostream& operator << (std::ostream& os, const vec2& v)
    { return os << v.x << ' ' << v.y; }

    // Input
    friend std::istream& operator >> (std::istream& is, vec2& v)
    { return is >> v.x >> v.y; }

};

// Standard inner product
GLfloat dot(const vec2& u, const vec2& v) {
    GLfloat val = u.x * v.x + u.y * v.y;

    if (std::abs(val) < zero_tolerance)
        return 0;
    else
        return val;
}

// Length of a vector
GLfloat length(const vec2& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}


//
// Three-dimensional real vector
//
struct vec3 {
    GLfloat x;
    GLfloat y;
    GLfloat z;

    // Constructor
    vec3 (GLfloat a = 0, GLfloat b = 0, GLfloat c = 0) {
        x = a, y = b, z = c;
        if (std::abs(a) < zero_tolerance) x = 0;
        if (std::abs(b) < zero_tolerance) y = 0;
        if (std::abs(c) < zero_tolerance) z = 0;
    }

    // Vector normalization
    void normalize() {
        double l = sqrt(x*x + y*y + z*z);
        x /= l;
        y /= l;
        z /= l;
    }

    void normalize(double r) {
        double l = sqrt(x*x + y*y + z*z);
        x = x / l * r;
        y = y / l * r;
        z = z / l * r;
    }

    //
    // Operator overloading
    //

    // Inverse
    vec3 operator - () const
    { return vec3(-x, -y, -z); }

    // Vector sum and difference
    friend vec3 operator + (const vec3& u, const vec3& v)
    { return vec3(u.x + v.x, u.y + v.y, u.z + v.z); }

    friend vec3 operator - (const vec3& u, const vec3& v)
    { return vec3(u.x - v.x, u.y - v.y, u.z - v.z); }

    // Multiplication by a scalar
    vec3 operator * (const GLfloat c) const { return vec3(c*x, c*y, c*z); }
    vec3 operator / (const GLfloat c) const { return vec3(x/c, y/c, z/c); }
    friend vec3 operator * (const GLfloat c, const vec3& v) { return v * c; }

    // Compound assignment
    vec3& operator += (const vec3& v)
    { *this = vec3(x + v.x, y + v.y, z + v.z); return *this; }

    vec3& operator -= (const vec3& v)
    { *this = vec3(x - v.x, y - v.y, z - v.z); return *this; }

    vec3& operator *= (const GLfloat c)
    { *this = vec3(x*c, y*c, z*c); return *this; }

    vec3& operator /= (const GLfloat c)
    { *this = vec3(x/c, y/c, z/c); return *this; }

    // Indexing operators
    GLfloat& operator [] (int i) { return *(&x + i); }
    const GLfloat operator [] (int i) const { return *(&x + i); }

    // Cross product
    friend vec3 operator * (const vec3& u, const vec3& v)
    {
        return vec3(
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        );
    }

    // Extraction operator
    friend std::ostream& operator << (std::ostream& os, const vec3& v)
    { return os << v.x << ' ' << v.y << ' ' << v.z; }

    // Input
    friend std::istream& operator >> (std::istream& is, vec3& v)
    { return is >> v.x >> v.y >> v.z; }
};

// Standard inner product
GLfloat dot(const vec3& u, const vec3& v) {
    GLfloat val = u.x * v.x + u.y * v.y + u.z * v.z;

    if (std::abs(val) < zero_tolerance)
        return 0;
    else
        return val;
}

// Length of a vector
GLfloat length(const vec3& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
