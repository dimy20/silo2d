
#include <Physics2D/Forces.h>
#include <glm/gtx/norm.hpp>

#define M_G 9.81f

glm::vec2 Physics2D::dragForce(const Physics2D::Particle& particle, float k){
    float magSquared = glm::length2(particle.mVelocity);

    if(magSquared == 0){
        return glm::vec2(0.0f);
    }

    glm::vec2 dir = -glm::normalize(particle.mVelocity);

    float dragMagnitude = k * magSquared;

    return dir * dragMagnitude;
};


glm::vec2 Physics2D::weight(const Physics2D::Particle& particle){
    return glm::vec2(0.0f, M_G * particle.mMass);
}
