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

/*********************
 * TEST 1:
 *********************/
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


/*********************
 * TEST 2:
 *********************/
TEST(EulerStepTests, SingleParticle_NoForces) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,0}));

    Integrator integrator(IntegrationMode::EulerStep, particles, 0.0f);

    integrator.update(1.0f);

    expect_vec_near(particles[0].position, {1,0});
    expect_vec_near(particles[0].velocity, {1,0});
}

/*********************
 * TEST 3:
 *********************/
TEST(EulerStepTests, ConstantAcceleration) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {0,0}));

    // manually inject acceleration AFTER gravity
    Integrator integrator(IntegrationMode::EulerStep, particles, 0.0f);

    particles[0].accel = {1, 0};

    integrator.update(1.0f);

    expect_vec_near(particles[0].velocity, {1,0});
    expect_vec_near(particles[0].position, {1,0});
}


/*********************
 * TEST 4:
 *********************/
TEST(LeapfrogTests, DtZero_NoChange) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,1}));

    Integrator integrator(IntegrationMode::LeapFrog, particles, 0.0f);

    auto initial_pos = particles[0].position;
    auto initial_vel = particles[0].velocity;

    integrator.update(0.0f);

    expect_vec_near(particles[0].position, initial_pos);
    expect_vec_near(particles[0].velocity, initial_vel);
}

/*********************
 * TEST 5:
 *********************/
TEST(LeapfrogTests, ZeroAcceleration) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,0}));

    Integrator integrator(IntegrationMode::LeapFrog, particles, 0.0f);

    particles[0].accel = {0,0};

    integrator.update(1.0f);

    expect_vec_near(particles[0].position, {1,0});
    expect_vec_near(particles[0].velocity, {1,0});
}


/*********************
 * TEST 6:
 *********************/
TEST(LeapfrogTests, FirstStepPrevAccelInitialization) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {0,0}));

    Integrator integrator(IntegrationMode::LeapFrog, particles, 0.0f);

    // If prev_accel is uninitialized, this may produce garbage
    integrator.update(1.0f);

    // Expected: no movement if accel = 0
    expect_vec_near(particles[0].position, {0,0});
    expect_vec_near(particles[0].velocity, {0,0});
}


/*********************
 * TEST 7:
 *********************/
TEST(RK4Tests, DtZero_NoChange) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,1}));

    Integrator integrator(IntegrationMode::RK4, particles, 0.0f);

    auto initial_pos = particles[0].position;
    auto initial_vel = particles[0].velocity;

    integrator.update(0.0f);

    expect_vec_near(particles[0].position, initial_pos);
    expect_vec_near(particles[0].velocity, initial_vel);
}

/*********************
 * TEST 8:
 *********************/
TEST(RK4Tests, SingleParticleConstantVelocity) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,0}));

    Integrator integrator(IntegrationMode::RK4, particles, 0.0f);

    integrator.update(1.0f);

    expect_vec_near(particles[0].position, {1,0});
}

/*********************
 * TEST 9:
 *********************/
TEST(RK4Tests, ZeroAcceleration) {
    std::vector<Particle> particles;
    particles.push_back(make_particle(1.0f, {0,0}, {1,0}));

    Integrator integrator(IntegrationMode::RK4, particles, 0.0f);

    particles[0].accel = {0,0};

    integrator.update(1.0f);

    expect_vec_near(particles[0].position, {1,0});
}

/*********************
 * TEST 10:
 *********************/
TEST(GravityTests, InsideCutoff_NoForce) {
    std::vector<Particle> particles;

    auto p1 = make_particle(1.0f, {0,0}, {0,0});
    auto p2 = make_particle(1.0f, {0.5f,0}, {0,0}); // very close

    particles.push_back(p1);
    particles.push_back(p2);

    Integrator integrator(IntegrationMode::EulerStep, particles, 1.0f);

    integrator.update(1.0f);

    expect_vec_near(particles[0].accel, {0,0});
}

/*********************
 * TEST 11:
 *********************/
TEST(GravityTests, OutsideCutoff_ForceApplied) {
    std::vector<Particle> particles;

    auto p1 = make_particle(1.0f, {0,0}, {0,0});
    auto p2 = make_particle(1.0f, {100,0}, {0,0});

    particles.push_back(p1);
    particles.push_back(p2);

    Integrator integrator(IntegrationMode::EulerStep, particles, 1.0f);

    integrator.update(1.0f);

    EXPECT_NE(particles[0].accel.norm(), 0.0f);
}