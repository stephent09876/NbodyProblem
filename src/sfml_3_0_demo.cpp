#include <SFML/Graphics.hpp>
#include <Eigen/Core>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <vector>

struct Body {
    float mass {10.0f};
    float radius {6.0f};
    Eigen::Vector2f position {0.0f, 0.0f};
    Eigen::Vector2f velocity {0.0f, 0.0f};
    Eigen::Vector2f accel {0.0f, 0.0f};
    sf::CircleShape shape;
};

struct EnergyState {
    double kinetic {0.0};
    double potential {0.0};
    double total {0.0};
};

constexpr float kG = 100.0f;
constexpr float kEnergyDriftThresholdPercent = 5.0f;

void applyGravity(std::vector<Body>& bodies) {
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        Eigen::Vector2f a_i{0.0f, 0.0f};

        for (std::size_t j = 0; j < bodies.size(); ++j) {
            if (i == j) {
                continue;
            }

            const Eigen::Vector2f q_ji = bodies[j].position - bodies[i].position;
            const float r = q_ji.norm();
            const float minDistance = bodies[i].radius + bodies[j].radius;
            if (r < minDistance) {
                continue;
            }

            // a_i = sum_{j!=i} G m_j (q_j - q_i) / ||q_j - q_i||^3
            a_i += (kG * bodies[j].mass / std::pow(r, 3.0f)) * q_ji;
        }

        bodies[i].accel = a_i;
    }
}

EnergyState calculateEnergy(const std::vector<Body>& bodies) {
    EnergyState state;
    for (const Body& b : bodies) {
        state.kinetic += 0.5 * static_cast<double>(b.mass) * static_cast<double>(b.velocity.squaredNorm());
    }

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies.size(); ++j) {
            const Eigen::Vector2f q_ji = bodies[j].position - bodies[i].position;
            const float r = q_ji.norm();
            const float minDistance = bodies[i].radius + bodies[j].radius;
            if (r < minDistance) {
                continue;
            }

            // U = -sum_{i<j} G m_i m_j / ||q_j - q_i||
            state.potential -= static_cast<double>(kG) *
                               static_cast<double>(bodies[i].mass) *
                               static_cast<double>(bodies[j].mass) /
                               static_cast<double>(r);
        }
    }
    state.total = state.kinetic + state.potential;
    return state;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1300, 800}), "SFML 3.0 N-body Demo");
    window.setFramerateLimit(60);

    std::vector<Body> bodies;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(150.0f, 1150.0f);
    std::uniform_real_distribution<float> yDist(120.0f, 680.0f);
    std::uniform_real_distribution<float> vDist(-5.0f, 5.0f);

    constexpr int numBodies = 20;
    bodies.reserve(numBodies);
    for (int i = 0; i < numBodies; ++i) {
        Body b;
        b.position = {xDist(gen), yDist(gen)};
        b.velocity = {vDist(gen), vDist(gen)};
        b.shape.setRadius(b.radius);
        b.shape.setOrigin({b.radius, b.radius});
        b.shape.setFillColor(sf::Color(255, 80, 80));
        b.shape.setPosition({b.position.x(), b.position.y()});
        bodies.push_back(b);
    }

    sf::Font font("bin/arial.ttf");
    sf::Text hud(font);
    hud.setCharacterSize(16);
    hud.setPosition({20.f, 20.f});
    hud.setFillColor(sf::Color::White);

    sf::Text warning(font);
    warning.setCharacterSize(16);
    warning.setPosition({20.f, 45.f});
    warning.setFillColor(sf::Color::Red);

    const EnergyState initialEnergy = calculateEnergy(bodies);
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        const float dt = clock.restart().asSeconds();
        applyGravity(bodies);

        for (Body& b : bodies) {
            b.velocity += b.accel * dt;
            b.position += b.velocity * dt;

            if (b.position.x() < b.radius || b.position.x() > 1300.0f - b.radius) {
                b.velocity.x() *= -1.0f;
            }
            if (b.position.y() < b.radius || b.position.y() > 800.0f - b.radius) {
                b.velocity.y() *= -1.0f;
            }

            b.shape.setPosition({b.position.x(), b.position.y()});
        }

        const EnergyState current = calculateEnergy(bodies);
        double driftPercent = 0.0;
        if (std::abs(initialEnergy.total) > std::numeric_limits<double>::epsilon()) {
            driftPercent = std::abs((current.total - initialEnergy.total) / initialEnergy.total) * 100.0;
        } else if (std::abs(current.total) > std::numeric_limits<double>::epsilon()) {
            driftPercent = std::numeric_limits<double>::infinity();
        }

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2)
               << "K: " << current.kinetic
               << " U: " << current.potential
               << " E0: " << initialEnergy.total
               << " E: " << current.total
               << " dE%: " << driftPercent;
        hud.setString(stream.str());

        if (driftPercent > kEnergyDriftThresholdPercent) {
            warning.setString("WARNING: Energy drift exceeds 5%");
        } else {
            warning.setString("");
        }

        window.clear(sf::Color::Black);
        for (const Body& b : bodies) {
            window.draw(b.shape);
        }
        window.draw(hud);
        window.draw(warning);
        window.display();
    }

    return 0;
}
