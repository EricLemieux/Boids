#ifndef FLOCK_H
#define FLOCK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Boid.h"

class Flock
{
    public:
        Flock();
        virtual ~Flock();

        void Update();

        Boid* members[100];
    protected:
        glm::vec3 position;
        glm::vec3 velocity;
    private:
};

#endif // FLOCK_H
