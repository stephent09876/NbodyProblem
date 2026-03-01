/**************************************************************************************************
 * File: NBodyProblem.cpp
 * Date: 2/22/2025
 * Authors:
 *          Stephen Thorsell
 *          Xiaohua Liu
 *          Shahzaib Memon
 * 
 * Description: This is the main execution script of the N Body Problem project being developed for
 *              Sprint 2026 SWEN 5239 Group XX. The following code sets up a simulation where a
 *              system of particles gravitationally interact with each other via newtons gravity
 *              equation. A graphics library is used to render the particles on the screen and a 
 *              user specified choice of numerical integrator (choices of Euler, Leapfrog, or RK4)
 *              is used to propagate the sim. 
 **************************************************************************************************/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <Eigen/Core>

// Internal project includes
#include "Particle.hpp"
#include "SimulationMode.hpp"
#include "Integrator.hpp"

// ---------------- Individual function declarations ---------------------------

/// @brief  getParticleCount - prompt a user input where the user specifies the total number of
/// particles to be ran in the simulation
int getParticleCount();

// ------------------- Set Simulation Variables -------------------------------------

// universal gravity constant. Tune to modify simulation response [unitless]
float G = 100;

// numerical integrator that is used to propagate the particle states (change at will here)
IntegrationMode integ_mode_slct = IntegrationMode::Leapfrog;

// size of window that SFML will generate
Eigen::Vector<unsigned int, 2> window_size {1300, 800};


// -------------------- MAIN FUNCTION -------------------------------------------
int main() {

    // ------------------- SIMULATION INITIALIZATION ----------------------------

    int numParticles = getParticleCount();

 

    sf::RenderWindow window(sf::VideoMode({window_size[0], window_size[1]}), "N-Body Gravity Simulation");
    window.setFramerateLimit(60);

    // Setup Randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posWidth(100.0f, 1200.0f);
    std::uniform_real_distribution<float> posHeight(100.0f, 700.0f);
    std::uniform_real_distribution<float> velDist(-0.0f, 0.0f);

    // Initialize Particle Vector
    std::vector<Particle> particles;

    for (int i = 0; i < numParticles; ++i) {
        Eigen::Vector2f startPos(posWidth(gen), posHeight(gen));
        Eigen::Vector2f startVel(velDist(gen), velDist(gen));

        // individual particle construction step
        particles.emplace_back(10.0f, 
                               sf::Color::Red, 
                               startPos, 
                               startVel, 
                               window_size.cast<float>());
                               
    }

    // initialize gravity model TEMPORARY: EVENTUALLY THIS WILL BE THE INTEGRATOR CLASS
    Integrator integrator(integ_mode_slct, particles, G);

    // set up a clock to track elapsed simulation time
    sf::Clock clock;
    sf::Time totalTime = sf::Time::Zero; 

    // set up a button to track whether the simulation is paused.
    SimulationMode sim_mode = SimulationMode::Running;
    bool mouse_button_pressed = false;

    sf::RectangleShape button(sf::Vector2f(120.f, 40.f));
    button.setPosition(sf::Vector2f(20.f, 20.f));
    button.setFillColor(sf::Color::Blue);

    sf::Font font("bin/arial.ttf");
    sf::Text buttonText(font);
    buttonText.setString("Pause");
    buttonText.setCharacterSize(18);
    buttonText.setPosition(sf::Vector2f(35.f, 25.f));
    buttonText.setFillColor(sf::Color::White);


    // -------------------- END SIM INITIALIZATION --------------------------------

    // -------------------- MAIN SIMULATION LOOP ----------------------------------

    while (window.isOpen()) {
        // reset button press mechanic
        mouse_button_pressed = false;

        // log time
        sf::Time dtTime = clock.restart();
        float dt = dtTime.asSeconds();
        totalTime += dtTime;

        // Check for 5-minute timeout (300 seconds)
        if (totalTime.asSeconds() >= 300.0f) {
            std::cout << "5-minute timeout reached. Terminating program." << std::endl;
            window.close();
            break;
        }

        // poll for new events
        while (const std::optional event = window.pollEvent()) {
            
            // poll for window closed event
            if (event->is<sf::Event::Closed>())
                window.close();

            // poll for button press event
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed ->button == sf::Mouse::Button::Left) {
                    mouse_button_pressed = true;
                }
            }
        }

        // switch simulation state if needed
        if (mouse_button_pressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (button.getGlobalBounds().contains(mousePos)) {

                // toggle simulation state
                switch (sim_mode) {
                    case SimulationMode::Running:
                        sim_mode = SimulationMode::Paused;
                        buttonText.setString("Resume");
                        break;
                    case SimulationMode::Paused:
                        sim_mode = SimulationMode::Running;
                        buttonText.setString("Pause");
                        break;
                }
            }
        }

        // Update Particle State
        if (sim_mode == SimulationMode::Running) {
            integrator.update(dt);

            for (Particle& p : particles) {
                p.update();
            }
        }

        // Reset window
        window.clear(sf::Color::Black);

        // Draw particles
        for (Particle& p : particles) {
            window.draw(p.shape);
        }
        
        // draw pause/play button
        window.draw(buttonText);
        
        window.display();
    }
    // ------------------ END MAIN SIMULATION LOOP ---------------------------------

    return 0;
}

// ---------------------- SUPPORTING FUNCTIONS -------------------------------------

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
