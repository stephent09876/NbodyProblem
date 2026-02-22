#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <Eigen/Core>

// --- Helper Function for User Input ---
int getParticleCount() {
    int count;
    std::cout << "Enter the number of particles (Max 100): ";
    if (!(std::cin >> count)) return 0;
    
    // Limit to 100 particles as requested
    if (count > 100) {
        std::cout << "Limit exceeded. Setting count to 100." << std::endl;
        return 100;
    }
    return std::max(0, count);
}

// ------------------- Universal Gravity Constant --------------------
float G = 100;
// -------------------------------------------------------------------

void gravityModel(std::vector<Particle> &p) {
    for (std::size_t idx = 0; idx < p.size(); idx++) {

        // reset acceleration vector
        Eigen::Vector2f acc{0.0, 0.0};

        for (std::size_t jdx = 0; jdx < p.size(); jdx++) {

            // skip if the ith particle and jth particle are the same
            if (idx == jdx) {
                continue;
            }

            Eigen::Vector2f r_ji;
            float           r_mag;

            // calculate position from the jth particle to the ith particle
            r_ji = p[idx].position - p[jdx].position;
            
            
            r_mag = r_ji.norm();

            if (r_mag < 2.0 * p[idx].radius) {
                continue;  // dont calculate an acceleration between these particles if they're too close
            }

            acc -= 1.0 * G * p[jdx].mass / std::pow(r_mag, 3) * r_ji; 
        }

        p[idx].accel = acc;
    }
}

int main() {

    int numParticles = getParticleCount();

    sf::RenderWindow window(sf::VideoMode({1300, 800}), "Eigen Multi-Particle Simulation");
    window.setFramerateLimit(60);

    // Setup Randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_real_distribution<float> massDist(5.0f, 100.0f);
    std::uniform_real_distribution<float> posWidth(100.0f, 1200.0f);
    std::uniform_real_distribution<float> posHeight(100.0f, 700.0f);
    std::uniform_real_distribution<float> velDist(-0.0f, 0.0f);
    //std::uniform_int_distribution<int> colorDist(50, 255);

    // Initialize Particle Vector
    std::vector<Particle> particles;

    for (int i = 0; i < numParticles; ++i) {
        Eigen::Vector2f startPos(posWidth(gen), posHeight(gen));
        Eigen::Vector2f startVel(velDist(gen), velDist(gen));
        //sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
        particles.emplace_back(10.0f, sf::Color::Red, startPos, startVel);
        //particles.emplace_back(massDist(gen), color, startPos, startVel);
    }

    sf::Clock clock;
    sf::Time totalTime = sf::Time::Zero; // Track total elapsed time
    while (window.isOpen()) {
        //float dt = clock.restart().asSeconds();
        sf::Time dtTime = clock.restart();
        float dt = dtTime.asSeconds();
        totalTime += dtTime;

        // Check for 5-minute timeout (300 seconds)
        if (totalTime.asSeconds() >= 300.0f) {
            std::cout << "5-minute timeout reached. Terminating program." << std::endl;
            window.close();
            break;
        }

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        gravityModel(particles);
        
        // Update and Draw all particles
        for (auto& p : particles) {
            p.update(dt);
            window.draw(p.shape);
        }
        
        window.display();
    }

    return 0;
}
