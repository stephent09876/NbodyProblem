/**************************************************************************************************
 * File: Particle.hpp
 * Date: 2/13/2025
 * Description: This file implements the particle class that is compilable with SFML 2.6.1
 **************************************************************************************************/
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Particle {
public: 
    float mass;
    float x, y;
    float velocityX, velocityY;
    sf::CircleShape shape;
    sf::Color color;

    // Added a default constructor so std::vector<Particle> particles(numParticles) works
    Particle() : mass(1.0f), x(0), y(0), velocityX(0), velocityY(0) {} 
    
    Particle(float mass, sf::Color color);
    void update(float deltaTime);
};