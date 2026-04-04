/**************************************************************************************************
 * File: TestingUtils.cpp
 * Date: 4/4/2026
 * Authors: Elijah Salandanan
 * 
 * Description: This file contains helper functions for testing the N Body Problem simulation.
 *              It includes utilities to compute total energy of particle systems, run energy
 *              conservation tests, and execute edge case scenarios to validate numerical
 *              integrators (Euler, Leapfrog, RK4). These functions allow evaluation of
 *              physical correctness, energy drift, and integrator stability under special cases.
 **************************************************************************************************/

#include "TestingUtils.hpp"

// ------------------------------
// Compute Total Energy
// ------------------------------
// TODO: ⚠️ Discrepancy Between Energy Calculations
// Note: computeTotalEnergy() and ValidationState.energy.total may differ for certain edge cases.
//       ValidationState skips potential energy contributions for overlapping or extremely close particles
//       (distance < sum of radii), whereas computeTotalEnergy currently includes all particle pairs.
//       This can cause large differences in "Overlapping" and "Very Close" edge case tests.
//       Consider reconciling the two methods in the future to ensure consistent energy validation.
float computeTotalEnergy(const std::vector<Particle>& particles, float G) {
    float totalKinetic = 0.0f;
    float totalPotential = 0.0f;

    for (const auto& p : particles)
        totalKinetic += 0.5f * p.mass * p.velocity.squaredNorm();

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Eigen::Vector2f rVec = particles[i].position - particles[j].position;
            float dist = rVec.norm();
            if (dist > 0.0f)
                totalPotential += -G * particles[i].mass * particles[j].mass / dist;
        }
    }

    return totalKinetic + totalPotential;
}

// ------------------------------
// Energy Conservation Test
// ------------------------------
/// This function runs a simulation for a given number of steps and checks energy conservation by comparing the initial and final total energy. 
// It uses both the computeTotalEnergy function and the ValidationState from the simulation to cross-validate results. 
// The energy drift percentage is calculated to assess the stability of the integrator under the specified conditions.
void runEnergyConservationTest(IntegrationMode mode, std::vector<Particle>& testParticles, float dt, int steps, float G_local) {
    std::cout << "=== Energy Conservation Test ===\n";

    GravityModel gravity(testParticles, G_local);
    Integrator integrator(mode, testParticles, G_local);

    ValidationState initialValidation = computeValidationState(testParticles, G_local);
    float E0_validation = initialValidation.energy.total;
    float E0_calc = computeTotalEnergy(testParticles, G_local);

    std::cout << "Initial Energy (ValidationState): " << E0_validation << "\n";
    std::cout << "Initial Energy (computeTotalEnergy): " << E0_calc << "\n";

    for (int i = 0; i < steps; ++i)
        integrator.update(dt);

    ValidationState finalValidation = computeValidationState(testParticles, G_local);
    float E_final_validation = finalValidation.energy.total;
    float E_final_calc = computeTotalEnergy(testParticles, G_local);

    float drift_validation = 100.0f * std::abs(E_final_validation - E0_validation) / std::abs(E0_validation);
    float drift_calc       = 100.0f * std::abs(E_final_calc - E0_calc) / std::abs(E0_calc);

    std::cout << "Final Energy (ValidationState): " << E_final_validation << "\n";
    std::cout << "Final Energy (computeTotalEnergy): " << E_final_calc << "\n";
    std::cout << "Energy Drift (%) [ValidationState]: " << drift_validation << "\n";
    std::cout << "Energy Drift (%) [computeTotalEnergy]: " << drift_calc << "\n";

    std::cout << "Initial Energy Difference: " << std::abs(E0_validation - E0_calc) << "\n";
    std::cout << "Final Energy Difference: " << std::abs(E_final_validation - E_final_calc) << "\n";

    std::cout << "=== End Energy Test ===\n\n";
}

// ------------------------------
// Run Edge Case Tests
// ------------------------------
/*
    Edge Case Tests for N-Body Simulation

    Purpose:
    These tests are designed to evaluate the stability, correctness, and numerical behavior
    of our N-body integrator under unusual or extreme conditions. Realistic scenarios are
    often well-behaved, but edge cases can expose bugs, instability, or energy drift
    in the simulation algorithms.

    Edge Cases:

    1. Zero Velocity
       - Particles start at rest.
       - Tests whether the integrator preserves energy and remains stable when no initial motion exists.

    2. Overlapping Particles
       - Two or more particles start at the exact same position.
       - Tests how the system handles potential singularities or division by zero in force calculations.

    3. Extreme Mass Ratios
       - One particle is much heavier than the others.
       - Checks numerical stability and correct handling of dominant-mass interactions.

    4. Very Close Particles
       - Particles placed extremely near each other.
       - Ensures forces do not blow up and the integrator handles small distances correctly.

    5. Very Far Particles
       - Particles placed far apart.
       - Verifies that weak interactions are still computed correctly and do not vanish numerically.

    6. High Timestep
       - A timestep significantly larger than normal.
       - Tests whether the integrator remains stable or shows energy drift under aggressive timesteps.

    7. 3+ Particle Interaction
       - Multiple particles in triangular or linear configurations.
       - Checks multi-body dynamics correctness and cumulative numerical effects.

    Using these edge cases, we can validate both energy conservation and numerical robustness
    across a variety of challenging scenarios in the simulation.
*/
void runEdgeCaseTests(IntegrationMode mode, float G_local) {
    std::cout << "=== Edge Case Tests ===\n";

    // 1. Zero Velocity
    std::vector<Particle> zeroVelParticles = {
        {1.0f, sf::Color::Red, Eigen::Vector2f(100,100), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::Red, Eigen::Vector2f(200,100), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[Zero Velocity]\n";
    runEnergyConservationTest(mode, zeroVelParticles);

    // 2. Overlapping Particles
    std::vector<Particle> overlappingParticles = {
        {1.0f, sf::Color::Blue, Eigen::Vector2f(300,300), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::Blue, Eigen::Vector2f(300,300), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[Overlapping Particles]\n";
    runEnergyConservationTest(mode, overlappingParticles);

    // 3. Extreme Mass Ratios
    std::vector<Particle> extremeMassParticles = {
        {1000.0f, sf::Color::Green, Eigen::Vector2f(400,400), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::Green, Eigen::Vector2f(450,400), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[Extreme Mass Ratios]\n";
    runEnergyConservationTest(mode, extremeMassParticles);

    // 4. Very Close Particles
    std::vector<Particle> closeParticles = {
        {1.0f, sf::Color::Yellow, Eigen::Vector2f(500,500), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::Yellow, Eigen::Vector2f(500.01f,500), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[Very Close Particles]\n";
    runEnergyConservationTest(mode, closeParticles);

    // 5. Very Far Particles
    std::vector<Particle> farParticles = {
        {1.0f, sf::Color::Cyan, Eigen::Vector2f(0,0), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::Cyan, Eigen::Vector2f(10000,0), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[Very Far Particles]\n";
    runEnergyConservationTest(mode, farParticles);

    // 6. High Timestep
    std::vector<Particle> highDtParticles = {
        {1.0f, sf::Color::Magenta, Eigen::Vector2f(600,600), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::Magenta, Eigen::Vector2f(650,600), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[High Timestep]\n";
    runEnergyConservationTest(mode, highDtParticles, 0.1f);

    // 7. 3+ Particle Interaction
    std::vector<Particle> multiParticles = {
        {1.0f, sf::Color::White, Eigen::Vector2f(700,700), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::White, Eigen::Vector2f(750,700), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10},
        {1.0f, sf::Color::White, Eigen::Vector2f(725,750), Eigen::Vector2f(0,0), Eigen::Vector2f(1300,800), 10}
    };
    std::cout << "[3+ Particle Interaction]\n";
    runEnergyConservationTest(mode, multiParticles);

    std::cout << "=== End Edge Case Tests ===\n\n";
}
