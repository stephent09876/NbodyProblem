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

    /// @brief GravityModel - default Constructor
    /// @param Particles_in - A vector of particle instances that the gravity model will generate
    ///                       accelerations for.
    /// @param G_in - The Universal Gravitational constant for the sim
    GravityModel(std::vector<Particle> &particles_in, float G_in);
    
    /// @brief main gravity model update function - The Newtownian gravity model. Runs through a 
    /// list of particles and calculates the gravitational acceleration for each individual 
    /// particle.
    void update();

    /// @brief Vector of particles
    std::vector<Particle> &p;

private:

    /// @brief Universal gravitation constant
    float G = 0.0;

    // 
};
