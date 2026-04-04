#pragma once
#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include "Particle.hpp"
#include "Integrator.hpp"
#include "Gravity.hpp"
#include "SimulationValidation.hpp"

// Compute total kinetic + potential energy
float computeTotalEnergy(const std::vector<Particle>& particles, float G = 100.0f);

// Run energy conservation test and compare ValidationState vs manual calculation
void runEnergyConservationTest(IntegrationMode mode, std::vector<Particle>& testParticles, float dt = 0.01f, int steps = 1000, float G_local = 100.0f);

// Run predefined edge case tests using runEnergyConservationTest
void runEdgeCaseTests(IntegrationMode mode, float G_local = 100.0f);