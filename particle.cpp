#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "main.h"
#include "simulation.h"
#include "particle.h"

using namespace std;
using namespace sf;


Particle::Particle(int Itype, vector<int> Ipos = { 0,0 }) {
	type = Itype;
	position = Ipos;
	if (type == 0) {
		color = Color(50, 50, 50);
	}
	else if (type == 1) {
		color = Color(200, 150, 50);
	}

	cout << "nouvelle particule de type " + to_string(type) << endl;
	
}