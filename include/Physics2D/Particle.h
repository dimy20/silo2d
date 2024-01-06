#pragma once

#include <glm/vec2.hpp>

namespace Physics2D{
    const float PIXELS_PER_METER = 50.0f;
    struct Particle{

        Particle();

        Particle(const glm::vec2& pos,
                float mass = 1.0f,
                const glm::vec2& velocity = glm::vec2(0.0f),
                const glm::vec2& acceleration = glm::vec2(0.0f));

            void integrate(float deltaTime);
            void applyForce(const glm::vec2& f);
            void clearForces();

        glm::vec2 mPosition;
        glm::vec2 mVelocity;
        glm::vec2 mAcceleration;
        float mMass;
        float mInvMass;
        glm::vec2 mNetForce;
    };
}


