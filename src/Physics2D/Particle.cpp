#include "Physics2D/Particle.h"

using namespace Physics2D;

Particle::Particle(const glm::vec2& pos,
                   float mass,
                   const glm::vec2& velocity,
                   const glm::vec2& acceleration){
    mPosition = pos;
    mVelocity = velocity * PIXELS_PER_METER;
    mAcceleration = acceleration * PIXELS_PER_METER;
    mMass = mass;
    mInvMass = 1.0f / mass ? mass > 0.0f : 0.0f;
    mNetForce = glm::vec2(0.0f);
}

void Particle::integrate(float deltaTime){
    mAcceleration = mNetForce * mInvMass * PIXELS_PER_METER; // F = mA

    mVelocity += mAcceleration * deltaTime;
    mPosition += mVelocity * deltaTime;

    clearForces();
}

void Particle::applyForce(const glm::vec2& f) { mNetForce += f; }
void Particle::clearForces() { mNetForce = glm::vec2(0.0f); }
