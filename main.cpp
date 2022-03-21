#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "main.h"
#include "simulation.h"

using namespace std;
using namespace sf;

Simulation simulation;

int main()
{
    int width = 600;
    int height = 500;
    


    sf::RenderWindow window(sf::VideoMode(width, height), "sandbox simulation");

    simulation.test();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {

                window.close();
            }
            simulation.inputHandler(event, window);

        }

        window.clear();
        window.display();
    }

    return 0;
}