#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;

class Simulation;

class Particle {
public:
	int type;
	Simulation* simulation;
	vector<int> position;
	vector<int> lastposition;
	Color color;
	float friction;
	int isFalingTime;
	bool moving;
	int Yvel;
	int Xvel;

	//a ajouter : couleur, inertie, 

	Particle(int Itype, vector<int> Ipos, Simulation* sim);
	void Sand(int x, int y);
	int GetRand(int a, int b);
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};