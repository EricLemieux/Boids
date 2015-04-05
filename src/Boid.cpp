#include "Boid.h"



Boid::Boid()
{
    trans = glm::mat4();

    pos.x = float(rand() % 200 - 100);
    pos.y = float(rand() % 200 - 100);
    pos.z = -50;

    target.x = 0;
    target.y = 0;
    target.z = -50;

    maxForce.x = 40.f;
    maxForce.y = 40.f;
    maxForce.z = 40.f;

    velocity.x = float(rand()% 500 - 250);
    velocity.y = float(rand()% 500 - 250);
    velocity.z = float(rand()% 500 - 250);

	currentPosition = pos;
	futurePosition = pos;
}

Boid::~Boid()
{
    //dtor
}

void Boid::Update(std::array<Boid*, BOID_COUNT> otherBoids)
{
    boidList = otherBoids;

    float minDist = 1.0f;
    float neighbourDist = 5;

    int seperationCount = 0;
    int alignmentCount = 0;

    steerValue = glm::vec3(0,0,0);
    glm::vec3 sum(0,0,0);
    glm::vec3 sum2(0,0,0);


    for(unsigned int i = 0; i < BOID_COUNT; ++i)
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
            sum2 += diff;
            ++alignmentCount;
        }

    }

    glm::vec3 targetForce = target - this->pos;
    targetForce = glm::normalize(targetForce) * maxForce;
    acceleration += targetForce * 5.0f;

    SeperationCalc(seperationCount);
    AlignmentCalc(alignmentCount, sum);
    CohesionCalc(alignmentCount, sum2);

    acceleration += glm::vec3(0.01f, 0.01f, 0.01f);

    velocity += acceleration * deltaTime;
    velocity = glm::normalize(velocity);
    velocity *= maxSpeed;
    pos += velocity * deltaTime;
    acceleration = glm::vec3(0,0,0);

    glm::vec3 newForward = glm::normalize(velocity);

    glm::vec3 oldUp = glm::vec3(trans[0][1], trans[1][1], trans[2][1]);

    glm::vec3 newRight = glm::cross(oldUp, newForward);
    glm::vec3 newUp = glm::cross(newForward, newRight);
	
	trans[0][0] = newRight[0];
	trans[0][1] = newRight[1];
	trans[0][2] = newRight[2];
	trans[1][0] = newUp[0];
	trans[1][1] = newUp[1];
	trans[1][2] = newUp[2];
	trans[2][0] = newForward[0];
	trans[2][1] = newForward[1];
	trans[2][2] = newForward[2];

    trans[3] = glm::vec4(pos, 1.0f);

	canDraw = true;
}

void Boid::RemoteUpdate(float t)
{
	timeSinceLastUpdate += t;
	glm::vec3 p = ((1 - timeSinceLastUpdate / expectedTimeBetweenUpdates)*currentPosition) + ((timeSinceLastUpdate / expectedTimeBetweenUpdates)*futurePosition);

	trans[3] = glm::vec4(p, 1.0f);
}

void Boid::SetRemote(glm::vec3 position, glm::vec3 newForward)
{
	timeSinceLastUpdate = 0.0f;

	previousPosition = currentPosition;
	currentPosition = glm::vec3(trans[3].x, trans[3].t, trans[3].z);
	futurePosition = (position - previousPosition) + position;

    glm::vec3 oldUp = glm::vec3(trans[0][1], trans[1][1], trans[2][1]);

    glm::vec3 newRight = glm::cross(oldUp, newForward);
    glm::vec3 newUp = glm::cross(newForward, newRight);

	trans[0][0] = newRight[0];
	trans[0][1] = newRight[1];
	trans[0][2] = newRight[2];
	trans[1][0] = newUp[0];
	trans[1][1] = newUp[1];
	trans[1][2] = newUp[2];
	trans[2][0] = newForward[0];
	trans[2][1] = newForward[1];
	trans[2][2] = newForward[2];

	canDraw = true;
}

void Boid::SeperationCalc(int count)
{
    if(count > 0)
    {
        steerValue = steerValue / glm::vec3(count);
    }

    if(glm::length(steerValue) > 0)
    {
        steerValue = glm::normalize(steerValue);
        steerValue = steerValue * maxSpeed;
        steerValue -= velocity;
        steerValue = glm::normalize(steerValue);
        steerValue *= maxForce;
    }

    acceleration += steerValue * 2.0f;
}

void Boid::AlignmentCalc(int count, glm::vec3 sum)
{
    if(count > 0 && glm::length(sum) > 0)
    {
        sum *= (float)1/(float)count;
        sum = glm::normalize(sum);
        sum *= maxSpeed;
        glm::vec3 steer = sum - velocity;
        if(glm::length(steer) > 0)
            steer = glm::normalize(steer);
        steer *= maxForce;

        acceleration += steer * 2.0f;
    }
}

void Boid::CohesionCalc(int count, glm::vec3 sum)
{
    if(count > 0 && glm::length(sum)>0)
    {
        sum *= (float)1/(float)count;
        sum = glm::normalize(sum);
        sum *= maxSpeed;
        glm::vec3 steer = sum - velocity;
        if(glm::length(steer) > 0)
            steer = glm::normalize(steer);
        steer *= maxForce;
        acceleration += steer * 3.0f;
    }

}
