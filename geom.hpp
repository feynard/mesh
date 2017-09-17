
// Delete half of this file
// Generating a sphere from tetrahedron

vec2* faces_spherical;
vec3* faces;        // Array of faces
int i = 0;          // Array index
int size = 0;


// Polar sphere representation
vec3 spherical_map(double theta, double phi) {
    return vec3(cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
}

// USE SPHERICAL COORDINATES
void split_triangle(vec3 a, vec3 b, vec3 c, int n) {
    if (n == 0) {
        faces[i++] = a;
        faces[i++] = b;
        faces[i++] = c;
        return;
    }

    vec3 x = (a + b) / 2;
    vec3 y = (b + c) / 2;
    vec3 z = (c + a) / 2;

    x.normalize(0.75);
    y.normalize(0.75);
    z.normalize(0.75);

    split_triangle(a, x, z, n - 1);
    split_triangle(x, b, y, n - 1);
    split_triangle(z, y, c, n - 1);
    split_triangle(x, y, z, n - 1);
}

vec3* tetrahedron_sphere(int n, double r)
{
    double a = r * sqrt(8) / 3, b = r / 3;

    vec3* tet = new vec3[4];
/*
    tet[0] = vec2(5 * pi / 6, - atan(3 / sqrt(8)));
    tet[1] = vec2(pi / 2, - atan(3 / sqrt(8)));
    tet[2] = vec2(2 * pi / 3, - atan(3 / sqrt(8)));
    tet[3] = vec2(0, pi/2);
*/
    tet[0] = vec3(a * cos(pi/6), - b, - a * sin(pi/6));
    tet[1] = vec3(0, -b, a);
    tet[2] = vec3(- a * cos(pi/6), - b, - a * sin(pi/6));
    tet[3] = vec3(0, r, 0);

    size = 3 * pow(4, n);
//    faces_spherical = new vec2[size];
    faces = new vec3[size];

    split_triangle(tet[0], tet[1], tet[2], n - 1);
    split_triangle(tet[1], tet[2], tet[3], n - 1);
    split_triangle(tet[2], tet[0], tet[3], n - 1);
    split_triangle(tet[0], tet[1], tet[3], n - 1);
/*
    for (int i = 0; i < size; i++)
        faces[i] = spherical_map(faces_spherical[i].x, faces_spherical[i].y);
*/
    number_of_points = size;
    return faces;
}

vec3* polar_sphere(int u, int v, double r)
{

    List <vec3> edges_list;

    for (int i = 0; i < u + 1; i++)
        for (int j = 0; j < v + 2; j++) {
            // Horizontal edge
            if (i < u && j != 0 && j != v + 1) {
                edges_list.push(r * spherical_map(
                    2 * i * pi / u, - pi / 2 + j * pi / (v + 1)));
                edges_list.push(r * spherical_map(
                    2 * (i + 1) * pi / u, - pi / 2 + j * pi / (v + 1)));
            }

            // Vertical edge
            if (j < v + 1 && i != u) {
                edges_list.push(r * spherical_map(
                    2 * i * pi / u, - pi / 2 + j * pi / (v + 1)));
                edges_list.push(r * spherical_map(
                    2 * i * pi / u, - pi / 2 + (j + 1) * pi / (v + 1)));
            }
        }

    int N = edges_list.length();
    vec3* edges = new vec3[N];

    for (int i = 0; i < N; i++)
        edges[i] = edges_list.pop_head();

    number_of_points = N;
    return edges;
}
