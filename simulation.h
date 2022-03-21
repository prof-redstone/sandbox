#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Simulation {

public:
	void test();


	int inputHandler(sf::Event event, sf::RenderWindow& window);

	void updateMove();
	void move();
	void render();

};











#endif