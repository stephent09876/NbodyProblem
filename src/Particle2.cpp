/**************************************************************************************************
 * File: Particle2.cpp
 * Date: 2/15/2025
 * Description: This file implements the particle class that is compilable with SFML 3.0
 **************************************************************************************************/

#include "Particle2.hpp"
#include <cmath> // For sqrt if you want to scale radius by area

Particle::Particle(float mass, sf::Color color) : mass(mass), color(color) {
    // Set radius based on mass. 
    // Using sqrt(mass) often looks more natural as it represents volume/area
    float radius = std::sqrt(mass) * 2.0f; 
    shape.setRadius(radius);
    shape.setOrigin(sf::Vector2f(radius, radius)); // Center the origin for better physics
    shape.setFillColor(color);

    // Initial random positions within the 1300x800 window
    x = (float)rand() / RAND_MAX * 1300;
    y = (float)rand() / RAND_MAX * 800;
    
    // Increased initial velocity so movement is visible
    velocityX = ((float)rand() / RAND_MAX - 0.5f) * 100.f;
    velocityY = ((float)rand() / RAND_MAX - 0.5f) * 100.f;
}

void Particle::update(float deltaTime) {

    // TODO: calculate velocity from acceleration as well
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Boundary bouncing updated for 1300x800 window
    if (x < 0 || x > 1300) velocityX *= -1.f;
    if (y < 0 || y > 800) velocityY *= -1.f;

    shape.setPosition(sf::Vector2f(x, y));
}