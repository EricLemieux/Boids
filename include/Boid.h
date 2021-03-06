#ifndef BOID_H
#define BOID_H

#include <array>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern float expectedTimeBetweenUpdates;

static const int BOID_COUNT = 100;

class Boid
{
    public:
        Boid();
        virtual ~Boid();

        inline glm::mat4 GetTransformation(){return trans;};

        void Update(std::array<Boid*, BOID_COUNT> otherBoids);
		void RemoteUpdate(float t);
		void SetRemote(glm::vec3 position, glm::vec3 newForward);

        inline glm::vec3 GetPos(){return pos;}
        inline glm::vec3 GetVelocity(){return velocity;}

		bool canDraw = false;

        glm::vec3 target;

    protected:
    private:
        glm::mat4 trans;
        float maxSpeed = 60.0f;

        const float deltaTime = 0.01f;

        glm::vec3 pos;

        std::array<Boid*, BOID_COUNT> boidList;

        void SeperationCalc(int count);
        void AlignmentCalc(int count, glm::vec3 sum);
        void CohesionCalc(int count, glm::vec3 sum);

        glm::vec3 steerValue;

        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 maxForce;

		glm::vec3 previousPosition;
		glm::vec3 currentPosition;
		glm::vec3 futurePosition;

		float timeSinceLastUpdate = 0.0f;
};

#endif // BOID_H
