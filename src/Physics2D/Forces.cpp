
#include <Physics2D/Forces.h>
#include <glm/fwd.hpp>
#include <glm/gtx/norm.hpp>
#include <stdio.h>

static const glm::vec2 ZERO_VEC2(0.0f);
#define M_G 9.81f

glm::vec2 Physics2D::dragForce(const Physics2D::Particle& particle, float k){
    float magSquared = glm::length2(particle.mVelocity);

    if(magSquared == 0){
        return ZERO_VEC2;
    }

    glm::vec2 dir = -glm::normalize(particle.mVelocity);

    float dragMagnitude = k * magSquared;

    return dir * dragMagnitude;
};


glm::vec2 Physics2D::weightForce(const Physics2D::Particle& particle){
    return glm::vec2(0.0f, M_G * particle.mMass);
}

glm::vec2 Physics2D::frictionForce(const Particle& particle, float coef){
    if(particle.mVelocity == ZERO_VEC2){
        return ZERO_VEC2;
    }

    glm::vec2 frictionDirection = -glm::normalize(particle.mVelocity);
    return coef * frictionDirection;
}
