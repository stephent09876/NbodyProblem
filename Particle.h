#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Particle {
public:
    sf::CircleShape shape;
    sf::Color color;
    float mass;
    float x, y;
    float velocityX, velocityY;

    // Added a default constructor so std::vector<Particle> particles(numParticles) works
    Particle() : mass(1.0f), x(0), y(0), velocityX(0), velocityY(0) {} 
    
    Particle(float mass, sf::Color color);
    void update(float deltaTime);
};