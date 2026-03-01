/**************************************************************************************************
 * File: Gravity.Cpp
 * Date: 2/28/2026
 * Description: This file implements a newtonian gravity model. Having its own implimentation makes it
 * much more amenable to having multiple different numerical integrators
 * SOURCES: https://en.wikipedia.org/wiki/N-body_simulation
 **************************************************************************************************/

#include "Gravity.hpp"

GravityModel::GravityModel(std::vector<Particle> &particles_in, 
                           float G_in) : 
                           p(particles_in),
                           G(G_in) {}


// update gravity model: run through list of particles and calculate all gravity accelerations
// that all the particles impose on each other                           
void GravityModel::update()
{
     for (std::size_t idx = 0; idx < p.size(); idx++) {

        // reset acceleration vector
        Eigen::Vector2f accel{0.0, 0.0};

        for (std::size_t jdx = 0; jdx < p.size(); jdx++) {

            // skip if the ith particle and jth particle are the same
            if (idx == jdx) {
                continue;
            }

            Eigen::Vector2f r_ji;
            float           r_mag;

            // calculate position vector from the jth particle to the ith particle
            r_ji = p[idx].position - p[jdx].position;
            r_mag = r_ji.norm();

            if (r_mag < 2.0 * p[idx].radius) {
                continue;  // dont calculate an acceleration between these particles if they're too close
            }

            // Calculate gravitational acceleration (minus sign is on purpose via chosen convention)
            accel -= 1.0 * G * p[jdx].mass / std::pow(r_mag, 3) * r_ji; 
        }

        p[idx].accel = accel;
    }
}