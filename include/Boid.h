#ifndef BOID_H
#define BOID_H

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Boid
{
    public:
        Boid();
        virtual ~Boid();

        inline glm::mat4 GetTransformation(){return trans;};

        void Update(std::array<Boid*, 100> otherBoids);
        void RemoteUpdate(glm::vec3 position, glm::vec3 newForward);

        inline glm::vec3 GetPos(){return pos;}
        inline glm::vec3 GetVelocity(){return velocity;}

		bool canDraw = false;

    protected:
    private:
        glm::mat4 trans;
        float maxSpeed = 7.0f;

        const float deltaTime = 0.01f;

        glm::vec3 pos;
        glm::vec3 target;

        std::array<Boid*, 100> boidList;

        void SeperationCalc(int count);
        void AlignmentCalc(int count, glm::vec3 sum);
        void CohesionCalc(int count, glm::vec3 sum);

        glm::vec3 steerValue;

        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 maxForce;
};

#endif // BOID_H
