/**************************************************************************************************
 * File: Particle2.hpp
 * Date: 2/15/2025
 * Description: This file implements the particle class that is compilable with SFML 3.0
 **************************************************************************************************/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <Eigen/Core>

class Particle {
public:
    Eigen::Vector2f position {0.0, 0.0};
    Eigen::Vector2f velocity {0.0, 0.0};
    Eigen::Vector2f accel {0.0, 0.0};
    float mass;
    sf::CircleShape shape;
    

    // default constructor
    Particle(float m, sf::Color color, Eigen::Vector2f pos, Eigen::Vector2f vel);

    // update function
    void update(float deltaTime);
    
};
