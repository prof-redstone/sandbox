#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "particle.h"

class Simulation {

public:
	bool ready = false;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;


	void test();
	int inputHandler(sf::Event event, sf::RenderWindow& window);
	void updateMove();
	void move();
	void render(sf::RenderWindow& window);

	//for test

	vector<int> pos = { 50,50 };
	Particle part = Particle(1, pos);

	Simulation();

};











#endif