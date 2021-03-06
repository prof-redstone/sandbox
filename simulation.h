#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "particle.h"

class Simulation {

public:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	bool mousePresse;
	int mouseStillPresse;
	int mouseType;
	int mouseLastX = -1;
	int mouseLastY = -1;
	bool pause;

	int sizePixel;
	int nbRows;
	int nbCols;
	std::vector<std::vector< Particle* >> particleCollect;
	std::vector<std::vector<int>> Moves;

	Simulation(int nbC, int nbR, int sp);

	void UpdateMove(sf::RenderWindow& window);
	void Move();
	void Render(sf::RenderWindow& window);
	sf::String InputHandler(sf::Event event, sf::RenderWindow& window);
	void CheckAndMove(int TMove, int T1, int T2, int X1, int Y1, int X2, int Y2);
	void AddMove(int TMove, int T1, int T2, int X1, int Y1, int X2, int Y2);
	bool ValidType(int a, int b, int c);
	void HandPlace(int x, int y, int type);
	std::vector<int> RArray(const int& max);
	void Replace(int type, int a, int b);
	void Swap(int x1, int y1, int x2, int y2);
	bool V(int x, int y);
	int GetRand(int a, int b);
	void PlaceBTW(int matrixX1, int matrixY1, int matrixX2, int matrixY2, int type, int stroke);
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};






#endif