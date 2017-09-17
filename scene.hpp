#include "mesh.hpp"

class Scene {

    List <Mesh*> objects;

public:

    void add_object(Mesh* G) {
        objects.push(G);
    }

    void draw() {
        for (objects.set_iterator(); objects.iterator(); objects.iterate())
            objects.get_iterator() -> draw();
    }

};
