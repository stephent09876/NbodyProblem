/**************************************************************************************************
 * File: Integrator.hpp
 * Date: 2/28/2025
 * Description: This file implements the numerical integrators for propagating the N-body simulator
 *              forward in time. The user has the choice of three diferent integrators (Euler Step,
 *              Leapfrog, and RK4). The default choice will be Leapfrog as many sources has deemed
 *              this integrator well suited for orbital mechanices because of the ability to 
 *              conserve the initial simulation's energy state very well. This class also 
 *              instantiates the gravity model.
 *              
 *              Sources for each type of integrator is below. 
 * SOURCES:
 *      Euler step - https://en.wikipedia.org/wiki/Euler_method
 *      Leapfrog   - https://en.wikipedia.org/wiki/Leapfrog_integration
 *      RK4        - https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
 **************************************************************************************************/

#pragma once

#include <Eigen/Core>
#include <vector>
#include "Gravity.hpp"
#include "Particle.hpp"

enum class IntegrationMode {
    EulerStep = 0,
    Leapfrog  = 1,
    RK4       = 2
};

class Integrator {
public:

    // default constructor
    Integrator(IntegrationMode mode_in, 
               std::vector<Particle> &p_in, 
               float G);

    // top level update function
    void update(float dt);


private:

   // Integration mode (CAUTION: this is changed to whatever the user set in MAIN at construction 
   // time!)
   IntegrationMode integration_mode = IntegrationMode::Leapfrog;

    // Gravity model
    GravityModel gravity;

    // supporting variable for the leapfrog acceleration
    Eigen::Matrix<float, 100, 2> prev_accel;

    // individual integration subroutines for the different types of integrators
    void EulerStep(float dt);
    void LeapFrog(float dt);
    void RK4(float dt);

};