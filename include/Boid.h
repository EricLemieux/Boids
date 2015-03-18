#ifndef BOID_H
#define BOID_H

#include <glm/glm.hpp>

class Boid
{
    public:
        Boid();
        virtual ~Boid();

        inline glm::mat4 GetTransformation(){return trans;};

        void Update();

        glm::vec3 influenceOrientation;

    protected:
    private:
        glm::mat4 trans;
        float speed;
};

#endif // BOID_H
