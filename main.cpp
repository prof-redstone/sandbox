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

Simulation simulation = Simulation();



int main()
{
    int width = 600;
    int height = 600;

    int iterationPerFrame = 1;

    sf::RenderWindow window(sf::VideoMode(width, height), "sandbox simulation");
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