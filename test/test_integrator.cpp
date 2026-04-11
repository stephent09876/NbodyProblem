#include <gtest/gtest.h>
#include "Integrator.hpp"
#include "Particle.hpp"

#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include <vector>
#include <iostream>

// -----------------------------
// Helper Functions
// -----------------------------

Particle make_particle(float mass,
                       Eigen::Vector2f pos,
                       Eigen::Vector2f vel,
                       Eigen::Vector2f bounds = {1300, 800}) {
    return Particle(mass, sf::Color::White, pos, vel, bounds, 1);
}

// Compare Eigen vectors with tolerance
void expect_vec_near(const Eigen::Vector2f& a,
                     const Eigen::Vector2f& b,
                     float tol = 1e-4f) {
    EXPECT_NEAR(a.x(), b.x(), tol);
    EXPECT_NEAR(a.y(), b.y(), tol);
}

TEST(EulerStepTests, DtZero_NoStateChange) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,1}));

    Integrator integrator(IntegrationMode::EulerStep, particles, 0.0f);

    auto initial_pos = particles[0].position;
    auto initial_vel = particles[0].velocity;

    integrator.update(0.0f);

    expect_vec_near(particles[0].position, initial_pos);
    expect_vec_near(particles[0].velocity, initial_vel);
}
