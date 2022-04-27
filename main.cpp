#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "main.h"
#include "simulation.h"
#include "particle.h"
#include "outils.h"

using namespace std;
using namespace sf;



/*
lien : 
https://www.youtube.com/watch?v=5Ka3tbbT-9E
https://www.youtube.com/watch?v=VLZjd_Y1gJ8

*/


int main()
{
    int width = 1000;
    int height = 600;
    int sizePixel = 4;

    Simulation simulation = Simulation(width, height, sizePixel);


    int iterationPerFrame = 3;

    sf::RenderWindow window(sf::VideoMode(width, height), "sandbox simulation", sf::Style::Close);
    window.setFramerateLimit(60);

    Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            String commande = simulation.InputHandler(event, window);
            if (commande == "FPS UP") {
                iterationPerFrame++;
            }
            if (commande == "FPS Down") {
                iterationPerFrame--;
            }
        }
        window.clear(Color::Black);

        for (int i = 0; i < iterationPerFrame; i++){
            simulation.UpdateMove(window);
            simulation.Move();
        }
        simulation.Render(window);

        Time time = clock.getElapsedTime();
        //cout << 1.0f / time.asSeconds() << endl;
        clock.restart().asSeconds();

        window.display();
    }

    return 0;
}

void Commande(String str) {
    
}