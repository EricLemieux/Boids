#include "Flock.h"

Flock::Flock()
{
    for(unsigned int i = 0; i < 100; ++i)
    {
        members[i] = new Boid();
    }

    position = glm::vec3(0,0,0);
    velocity = glm::vec3(0,0,0);
}

Flock::~Flock()
{
    //dtor
}

void Flock::Update()
{
    for(unsigned int i = 0; i < 100; ++i)
    {
        //Sort the boids

        //Update the boids influenced orientation
        //TODO

        //Update the boids transformation
        members[i]->Update();
    }
}
