
#include <glm/vec2.hpp>
#include <Physics2D/Particle.h>

namespace Physics2D{
    const float UNI_GRAVITATIONAL_CONST = 6.67430e-11; // Using scientific notation

    // Utility function to generate a drag force vector
    glm::vec2 dragForce(const Physics2D::Particle& particle, float k);
    glm::vec2 weightForce(const Physics2D::Particle& particle);
    glm::vec2 frictionForce(const Particle& particle, float coef); // coef = ||normal|| * mu
    glm::vec2 make_gravitational_force(const Particle& p1, const Particle& p2,
                                    float G = UNI_GRAVITATIONAL_CONST);
};
