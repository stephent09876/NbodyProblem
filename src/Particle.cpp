/**************************************************************************************************
 * File: Particle2.cpp
 * Date: 2/15/2025
 * Description: This file implements the particle class that is compilable with SFML 3.0
 **************************************************************************************************/

#include "Particle.hpp"
#include <cmath> // For sqrt if you want to scale radius by area

Particle::Particle(float m, sf::Color color, Eigen::Vector2f pos, Eigen::Vector2f vel) 
        : mass(m), position(pos), velocity(vel) {
        
        // Visual representation setup
        float radius = std::sqrt(mass) * 2.0f;
        shape.setRadius(radius);
        shape.setOrigin({radius, radius}); // Center the origin for Eigen mapping
        shape.setFillColor(color);
    }

void Particle::update(float deltaTime) {
    // Linear motion: P_new = P_old + V * dt
    position += velocity * deltaTime;

    // Boundary bouncing logic (Window: 1300x800)
    if (position.x() < 0 || position.x() > 1300) velocity.x() *= -1.0f;
    if (position.y() < 0 || position.y() > 800)  velocity.y() *= -1.0f;

    // Sync SFML shape with Eigen position
    shape.setPosition({position.x(), position.y()});
}