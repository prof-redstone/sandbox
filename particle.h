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

	//a ajouter : couleur, inertie, 


	Particle(int Itype, vector<int> Ipos);

};