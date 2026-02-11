/******************************************************************************
 * File: sfml_demo_stephen
 * Date: 2/10/25
 * Description: This file demos the ability to create a window, draw a circle, 
 * and sometime later, delete the circle and draw a new circle somewhere else.
 * The purpose of this is to confirm that we can actually do an animation by 
 * updating the "frame" of the animation. This is a rendition of the classic
 *  DVD logo bouncing off of the walls of the window (DO NOT RESIZE WINDOW LIVE)
 * 
 *******************************************************************************/

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    unsigned int window_length = 1300;
    unsigned int window_height = 800;
    float r = 5.0f;     // circle radius


    sf::RenderWindow window(sf::VideoMode({window_length, window_height}), 
                                            "Animation demo works!");

    // positioning window closer to the corner of the screen
    window.setPosition(sf::Vector2i(10, 10)); 
    window.setFramerateLimit(60);   // setting frame rate 
    sf::CircleShape shape(r);
    shape.setPosition(sf::Vector2f(0.0, 0.0));
    shape.setFillColor(sf::Color::White);

    // position variables to update
    float x_pos = 0.0;
    float y_pos = 0.0;

    float x_vel = 2.0;
    float y_vel = 2.0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }


        // draw shape
        window.clear();
        shape.setPosition(sf::Vector2f(x_pos, y_pos));
        window.draw(shape);
        window.display();

        // update velocity if circle bounces of boundary
        if (x_pos < 0.0 || x_pos > (static_cast<float>(window_length) - 2.0*r)) {
            x_vel = -1.0 * x_vel;
        }
        if (y_pos < 0.0 || y_pos > (static_cast<float>(window_height) - 2.0*r)) {
            y_vel = -1.0 * y_vel;
        }

        // update animation dynamics
        x_pos = x_pos + x_vel;
        y_pos = y_pos + y_vel;
    }
}