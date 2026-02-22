#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
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

int main() {

    Eigen::Vector2f A;
    A << 3, 4;

    Eigen::Vector2f B;
    B << 7, 6;

    Eigen::Vector2f C = A - B;

    float vector_mag = C.norm();

    std::cout << C << std::endl;
    std::cout << vector_mag << std::endl;

    std::cout << "this is just a demo that Eigen works:" << std::endl;
    std::cout << A << std::endl;
    std::cout << std::endl;


    int numParticles = getParticleCount();

    sf::RenderWindow window(sf::VideoMode({1300, 800}), "Eigen Multi-Particle Simulation");
    window.setFramerateLimit(60);

    // Setup Randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_real_distribution<float> massDist(5.0f, 100.0f);
    std::uniform_real_distribution<float> posWidth(100.0f, 1200.0f);
    std::uniform_real_distribution<float> posHeight(100.0f, 700.0f);
    std::uniform_real_distribution<float> velDist(-150.0f, 150.0f);
    //std::uniform_int_distribution<int> colorDist(50, 255);

    // Initialize Particle Vector
    std::vector<Particle> particles;
    for (int i = 0; i < numParticles; ++i) {
        Eigen::Vector2f startPos(posWidth(gen), posHeight(gen));
        Eigen::Vector2f startVel(velDist(gen), velDist(gen));
        //sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
        particles.emplace_back(40.0f, sf::Color::Red, startPos, startVel);
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
        
        // Update and Draw all particles
        for (auto& p : particles) {
            p.update(dt);
            window.draw(p.shape);
        }
        
        window.display();
    }

    return 0;
}
