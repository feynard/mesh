#include "mesh.hpp"

class Scene {

    List <Mesh> objects;

public:
    Scene() {

    }

    void add_object(Mesh object);
    /*
    void draw_scence() {
        for (all objects)
            object.draw();

        draw_grid()
    }
    */
};
