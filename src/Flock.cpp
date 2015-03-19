#include "Flock.h"

Flock::Flock()
{
	for (unsigned int i = 0; i < BOID_COUNT; ++i)
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

void Flock::Update(glm::vec3 newTarget)
{
	for (unsigned int i = 0; i < BOID_COUNT; ++i)
    {
        members[i]->target = newTarget;
        members[i]->Update(members);
    }
}
