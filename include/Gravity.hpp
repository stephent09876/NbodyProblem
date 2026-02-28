/**************************************************************************************************
 * File: Gravity.hpp
 * Date: 2/28/2026
 * Description: This file implements a newtonian gravity model. Having its own impimentation makes it
 * much more amenable to having multiple different numerical integrators
 **************************************************************************************************/

#pragma once

#include <vector>
#include <Eigen/Core>
#include "Particle.hpp"

class GravityModel {

public:

    /// @brief gravityModel - The Newtownian gravity model. Runs through a list of particles and
    /// calculates the gravitational acceleration for each individual particle.
    GravityModel(std::vector<Particle> &particles_in, float G_in);
    
    // main gravity model update function
    void update();

private:
    
    /// @brief Vector of particles
    std::vector<Particle> &p;

    /// @brief Universal gravitation constant
    float G = 0.0;

    // 
};
