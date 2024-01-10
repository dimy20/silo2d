
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

glm::vec2 Physics2D::make_gravitational_force(const Particle& p1, const Particle& p2, float G){
    glm::vec2 direction = glm::normalize(p2.mPosition - p1.mPosition);

    float p1p2_massses = p2.mMass * p1.mMass;

    if(glm::length2(direction) == 0.0f){
        return glm::vec2(0.0f);
    };
    
    float inv_direction_length_squared = 1.0f / glm::length2(direction);

    float magnitude = G * p1p2_massses * inv_direction_length_squared;

    return magnitude * direction;
};
