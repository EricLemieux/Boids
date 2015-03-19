#include "Boid.h"

Boid::Boid()
{
    trans = glm::mat4();

    pos.x = float(rand()*2);
    pos.y = float(rand()*2);
    pos.z = float(rand()*2);

    velocity.x = float(rand());
    velocity.y = float(rand());
    velocity.z = float(rand());
}

Boid::~Boid()
{
    //dtor
}

void Boid::Update(std::array<Boid*, 100> otherBoids)
{
    boidList = otherBoids;

    float minDist = 1.0f;
    float neighbourDist = 1;

    int seperationCount = 0;
    int alignmentCount = 0;

    steerValue = glm::vec3(0,0,0);
    glm::vec3 sum(0,0,0);

    for(unsigned int i = 0; i < 100; ++i)
    {
        glm::vec3 diff = this->pos - boidList[i]->GetPos();
        float d = glm::length(diff);

        if(d < minDist && this != boidList[i])
        {
            glm::normalize(diff);
            diff = glm::vec3(diff.x/d, diff.y/d, diff.z/d);
            steerValue += diff;
            ++seperationCount;
        }

        if(d > 0 && d < neighbourDist)
        {
            sum += boidList[i]->GetVelocity();
            ++alignmentCount;
        }

    }

    SeperationCalc(seperationCount);
    AlignmentCalc(alignmentCount, sum);
    CohesionCalc(alignmentCount, sum);

    acceleration = glm::vec3(0.01f, 0.01f, 0.01f);

    velocity = acceleration * deltaTime;
    velocity = glm::normalize(velocity);
    velocity *= maxSpeed;
    pos = velocity * deltaTime;
    acceleration = glm::vec3(0,0,0);

    glm::vec3 newForward = glm::normalize(velocity);

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

    trans[3] = glm::vec4(pos, 1.0f);
}

void Boid::SeperationCalc(int count)
{
    if(count > 0)
    {
        steerValue = steerValue / glm::vec3(count);
    }

    if(glm::abs(glm::length(steerValue)) > 0)
    {
        steerValue = glm::normalize(steerValue);
        steerValue = steerValue * maxSpeed;
        steerValue -= velocity;
        steerValue = glm::normalize(steerValue);
        steerValue *= maxForce;
    }

    acceleration += steerValue;
}

void Boid::AlignmentCalc(int count, glm::vec3 sum)
{
    if(count > 0)
    {
        sum *= 1/count;
        sum = glm::normalize(sum);
        sum *= maxSpeed;
        glm::vec3 steer = sum - velocity;
        steer = glm::normalize(steer);
        steer *= maxForce;

        acceleration += steer;
    }
}

void Boid::CohesionCalc(int count, glm::vec3 sum)
{
    if(count > 0)
    {
        sum *= 1/count;
        sum = glm::normalize(sum);
        sum *= maxSpeed;
        glm::vec3 steer = sum - velocity;
        steer = glm::normalize(steer);
        steer *= maxForce;
        acceleration += steer;
    }

}
