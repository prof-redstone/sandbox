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

	int T(int x, int y);

	//for multi type :
	int Yvel;
	int Xvel;

	//for movable solid like sand :
	float friction;
	float inertieTransfer;
	int isFalingTime;
	bool moving;

	//for liquide:
	int pressure;
	


	Particle(int Itype, vector<int> Ipos, Simulation* sim);
	bool CanMove(int x, int y, int type);
	void Sand(int x, int y);
	void Water(int x, int y);
	void TransferInertia(int x, int y);
	void RecivedInertia();
	int GetRand(int a, int b);
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};