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

	int getRand(int a, int b);
	void replace(int type, int a, int b);
	void swap(int x1, int y1, int x2, int y2);
	String inputHandler(sf::Event event, sf::RenderWindow& window);
	void updateMove();
	void CheckAndMove(int Tmove, int T1, int T2, int X1, int Y1, int X2, int Y2);
	void move();
	void render(sf::RenderWindow& window);

	//for test

	int nbRows;
	int nbCols;
	vector<vector<Particle*>> particleCollect;
	vector<vector<int>> moves;
	bool V(int x, int y);
	void AddMove(int Tmove, int T1, int T2, int X1, int Y1, int X2, int Y2);
	bool CheckMove(int a, int b, int c);
	void Sand(int x, int y);
	void HandPlace(int x, int y, int type);


	Simulation();

};






#endif