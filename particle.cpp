#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "main.h"
#include "particle.h"
#include "outils.h"

using namespace std;
using namespace sf;

int getRand(int a, int b);

enum type {
	air,
	sand,
	water,
	stone,
	acide
};


Particle::Particle(int Itype, vector<int> Ipos, vector<vector<Particle*>> *particleCollect) {
	type = Itype;
	position = Ipos;
	particleCollect = particleCollect;

	//cout << this << endl;
	srand((unsigned)this);
	//cout << getRand(5,10);

	if (type == air) {
		color = Color(50, 50, 50);
	}
	else if (type == sand) {
		color = Color(getRand(200, 210), 150, 50);
	}
}

int getRand(int a, int b) {
	return (rand() % (b - a)) + a;
}

void Particle::checkmove() {
	//cout << "oui";
}