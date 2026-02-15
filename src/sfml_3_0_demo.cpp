#include "Particle2.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>

int main() {
    sf::RenderWindow window(sf::VideoMode({1300, 800}), "Particle Demo");
    window.setPosition(sf::Vector2i(10, 10)); 
    window.setFramerateLimit(60);

    int numParticles;
    std::cout << "Enter the number of particles: ";
    if (!(std::cin >> numParticles)) return -1;

    std::vector<Particle> particles(numParticles);

    std::random_device rd;
    std::mt19937 gen(rd());
    // Increased mass range to make size differences more obvious
    std::uniform_real_distribution<float> massDist(5.0f, 100.0f);  
    std::uniform_int_distribution<int> colorDist(0, 255);

    for (int i = 0; i < numParticles; ++i) {
        float mass = massDist(gen);
        sf::Color color;
        color.r = colorDist(gen);
        color.g = colorDist(gen);
        color.b = colorDist(gen);
        particles[i] = Particle(mass, color);
    }

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds(); // Calculate time passed per frame

        while ( const std::optional event = window.pollEvent() )
        {
            if ( event->is<sf::Event::Closed>() )
                window.close();
            }

        window.clear(sf::Color::Black);
        for (Particle& p : particles) {
            p.update(dt);
            window.draw(p.shape); // FIX: Draw the .shape member
        }
        window.display();
    }
    return 0;
}

// int main()
// {
// 	sf::RenderWindow window( sf::VideoMode( { 200, 200 } ), "SFML works!" );
// 	sf::CircleShape shape( 100.f );
// 	shape.setFillColor( sf::Color::Red );

// 	while ( window.isOpen() )
// 	{
// 		while ( const std::optional event = window.pollEvent() )
// 		{
// 			if ( event->is<sf::Event::Closed>() )
// 				window.close();
// 		}

// 		window.clear();
// 		window.draw( shape );
// 		window.display();
// 	}
// }
