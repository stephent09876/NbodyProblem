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

    // state vectors
    Eigen::Vector2f position {0.0, 0.0};
    Eigen::Vector2f velocity {0.0, 0.0};
    Eigen::Vector2f accel {0.0, 0.0};

    // borders that will bound the particle position
    Eigen::Vector2f boundary {0.0, 0.0};

    float mass = 0.0;
    float radius = 0.0;

    sf::CircleShape shape;
    

    // default constructor
    Particle(float m, 
             sf::Color color, 
             Eigen::Vector2f pos, 
             Eigen::Vector2f vel, 
             Eigen::Vector2f bounds);

    // update function
    void update(float deltaTime);
    
};
