
#include <glm/vec2.hpp>
#include <Physics2D/Particle.h>

namespace Physics2D{
    // Utility function to generate a drag force vector
    glm::vec2 dragForce(const Physics2D::Particle& particle, float k);
    glm::vec2 weight(const Physics2D::Particle& particle);
};