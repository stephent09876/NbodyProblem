/**************************************************************************************************
 * File: Integrator.cpp
 * Date: 2/28/2025
 * Description: This file implements the numerical integrators for propagating the N-body simulator
 *              forward in time. The user has the choice of three different integrators (Euler Step,
 *              Leapfrog, and RK4). The default choice will be Leapfrog as many sources has deemed
 *              this integrator well suited for orbital mechanices because of the ability to 
 *              conserve the initial simulation's energy state very well. This class also 
 *              instantiates the gravity model.
 *              
 *              Integrators ranked by difficulty to understand whats going on:
 *              Euler step - EASY
 *              Leapfrog   - MEDIUM
 *              RK4        - HARD
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
                       integration_mode(mode_in),
                       NUM_PARTICLES(p_in.size()) {}


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
        case IntegrationMode::LeapFrog:
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

    // calculate position
    for (Particle &p : gravity.p) {
        p.position += p.velocity*dt + 0.5*p.accel*std::pow(dt, 2.0);
    }

    // save off previous accelerations
    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        prev_accel.row(idx) = gravity.p[idx].accel;
    }

    // step gravity model
    gravity.update();

    // compute velocity
    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        gravity.p[idx].velocity += 0.5*(prev_accel.row(idx).transpose() + gravity.p[idx].accel)*dt;
    }

}


void Integrator::RK4(float dt) {

    /********************************************************************************
     * STATE VECTOR EXPLANATION
     * Each particle has a position: [x, y], velocity: [vx, vy] and acceleration: [ax, ay]
     * The total size of the state vector then becomes 4N where the state vector, X, gets constructed 
     * as follows:
     * 
     * X = [x_1, y_1, x_2, y_2, ... x_n, y_n, vx_1, vy_1, ..., vx_n, vy_n]
     * 
     * It should follow for a simulation of 20 particles, the state vector is of size 80 for example.
     * The time derivative of the state vector, Xdot, then becomes:
     * 
     * Xdot = [vx_1, vy_1, ..., vx_n, vy_n, ax_1, ay_1, ..., ax_n, ay_n]
     * 
     * The time derivative then forms the basis of whats being calculated with the K variables
     ********************************************************************************/

    /// TODO: this will probably break anytime num_particles < 2

    int state_vector_size = 4*NUM_PARTICLES;

    // RK4 state vector at the current time step.
    Eigen::VectorXf Xk(state_vector_size);

    // RK4 state vector at the future time step
    Eigen::VectorXf Xkplus1(state_vector_size);

    // initialize K variables
    Eigen::VectorXf K1(state_vector_size);
    Eigen::VectorXf K2(state_vector_size);
    Eigen::VectorXf K3(state_vector_size);
    Eigen::VectorXf K4(state_vector_size);

    // intermediate variable to help support the K variable computations
    Eigen::VectorXf X_modded(state_vector_size);

    int midpoint = state_vector_size/2;

    // Create the state vector at the current time step
    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        Xk.block<2, 1>(2*idx, 0)          = gravity.p[idx].position;
        Xk.block<2, 1>(midpoint+2*idx, 0) = gravity.p[idx].velocity;
    }

    /************************ K1 computation ***********************/
    gravity.update();

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        K1.block<2, 1>(2*idx, 0)            = gravity.p[idx].velocity;
        K1.block<2, 1>(midpoint + 2*idx, 0) = gravity.p[idx].accel;

    }

    /************************ K2 computation ***********************/
    X_modded = Xk + 0.5*K1*dt;

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        gravity.p[idx].position = X_modded.block<2, 1>(2*idx, 0);
        gravity.p[idx].velocity = X_modded.block<2, 1>(midpoint + 2*idx, 0);
    }

    gravity.update();

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        K2.block<2, 1>(2*idx, 0)            = gravity.p[idx].velocity;
        K2.block<2, 1>(midpoint + 2*idx, 0) = gravity.p[idx].accel;

    }
    
    /************************ K3 computation ***********************/
    X_modded = Xk + 0.5*K2*dt;

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        gravity.p[idx].position = X_modded.block<2, 1>(2*idx, 0);
        gravity.p[idx].velocity = X_modded.block<2, 1>(midpoint + 2*idx, 0);
    }

    gravity.update();

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        K3.block<2, 1>(2*idx, 0)            = gravity.p[idx].velocity;
        K3.block<2, 1>(midpoint + 2*idx, 0) = gravity.p[idx].accel;

    }

    /************************ K4 computation ***********************/
    X_modded = Xk + K3*dt;

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        gravity.p[idx].position = X_modded.block<2, 1>(2*idx, 0);
        gravity.p[idx].velocity = X_modded.block<2, 1>(midpoint + 2*idx, 0);
    }

    gravity.update();

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        K3.block<2, 1>(2*idx, 0)            = gravity.p[idx].velocity;
        K3.block<2, 1>(midpoint + 2*idx, 0) = gravity.p[idx].accel;

    }

    /**************** Runge-Kutta State Update *****************/
    Xkplus1 = Xk + dt/6.0 * (K1 + 2.0*K2 + 2.0*K3 + K4);

    for (std::size_t idx = 0; idx < NUM_PARTICLES; idx++) {
        gravity.p[idx].position = Xkplus1.block<2, 1>(2*idx, 0);
        gravity.p[idx].velocity = Xkplus1.block<2, 1>(midpoint + 2*idx, 0);
    }
}