/**************************************************************************************************
 * File: Integrator.cpp
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

#include "Integrator.hpp"
#include <cmath>   // std::pow

// default constructor
Integrator::Integrator(IntegrationMode mode_in, 
                       std::vector<Particle> &p_in, 
                       float G_in) :
                       gravity(p_in, G_in) ,
                       integration_mode(mode_in) {}


void Integrator::update(float dt) {

    // switch between the different integrators
    switch(integration_mode) {
        case IntegrationMode::EulerStep:
            EulerStep(dt);
            break;
        
        case IntegrationMode::RK4:
            RK4(dt);
            break;

        default:
        case IntegrationMode::Leapfrog:
            LeapFrog(dt);
            break;
    }
    
}



void Integrator::EulerStep(float dt) {

    gravity.update();

    // update all particles with euler step
    for (Particle &p : gravity.p) {
        p.velocity += p.accel * dt;
        p.position += p.velocity * dt;
    }
}

void Integrator::LeapFrog(float dt) {

   std::cout << "I am in leapfrog" << std::endl;
    
    // calculate position
    for (Particle &p : gravity.p) {
        p.position += p.velocity*dt + 0.5*p.accel*std::pow(dt, 2.0);
    }

    // save off previous accelerations
    for (std::size_t idx = 0; idx < gravity.p.size(); idx++) {
        prev_accel.row(idx) = gravity.p[idx].accel;
    }

    // step gravity model
    gravity.update();

    // compute velocity
    for (std::size_t idx = 0; idx < gravity.p.size(); idx++) {
        gravity.p[idx].velocity += 0.5*(prev_accel.row(idx).transpose() + gravity.p[idx].accel)*dt;
    }

}

void Integrator::RK4(float dt) {
    /// TODO: implement RK4 integrator 
}