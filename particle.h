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
	vector<vector<Particle*>>* particleCollect; //passe par simulation
	int falling; //0:cant fall, 1:can fall, 2:falling
	int weight;


	//a ajouter : couleur, inertie, 


	Particle(int Itype, vector<int> Ipos, vector<vector<Particle*>> *particleCollect);
	void checkmove();

};