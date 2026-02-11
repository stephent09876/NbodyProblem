/******************************************************************************
 * File: sfml_demo_stephen
 * Date: 2/10/25
 * Description: This file demos the ability to create a window, draw a circle, 
 * and sometime later, delete the circle and draw a new circle somewhere else.
 * The purpose of this is to confirm that we can actually do an animation by 
 * updating the "frame" of the animation.
 * 
 *******************************************************************************/

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1300,800}), "SFML works!");

    window.setPosition(sf::Vector2i(10, 10)); // positioning window closer to the corner of the screen
    window.setFramerateLimit(30);   // setting frame rate 
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}