#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>


using namespace std;
using namespace sf;



class Particle {
public:
	int type;
	vector<int> position;
	Color color;
	int falling; //0:cant fall, 1:can fall, 2:falling
	int weight;


	//a ajouter : couleur, inertie, 

	Particle(int Itype, vector<int> Ipos);

	int GetRand(int a, int b);
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};