#include "Boid.h"

Boid::Boid()
{
    trans = glm::mat4();
    speed = 0.001f;

    influenceOrientation.x = float(rand()*2);
    influenceOrientation.y = float(rand()*2);
    influenceOrientation.z = float(rand()*2);
}

Boid::~Boid()
{
    //dtor
}

void Boid::Update()
{
    glm::vec3 newForward = influenceOrientation + glm::vec3(trans[0][2], trans[1][2], trans[2][2]);
    newForward = glm::normalize(newForward);

    glm::vec3 oldUp = glm::vec3(trans[0][1], trans[1][1], trans[2][1]);

    glm::vec3 newRight = glm::cross(oldUp, newForward);
    glm::vec3 newUp = glm::cross(newForward, newRight);

    trans[0][0] = newRight[0];
    trans[0][1] = newUp[0];
    trans[0][2] = newForward[0];
    trans[1][0] = newRight[1];
    trans[1][1] = newUp[1];
    trans[1][2] = newForward[1];
    trans[2][0] = newRight[2];
    trans[2][1] = newUp[2];
    trans[2][2] = newForward[2];

    trans[3] += glm::vec4(newForward * speed, 0.0f);
}
