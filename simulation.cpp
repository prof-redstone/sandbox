#include "simulation.h"
#include <iostream>


using namespace sf;
using namespace std;

void Simulation::test()
{

	std::cout << "oui ca marche !" << std::endl;
}



void Simulation::updateMove()
{
}

void Simulation::move()
{
}

void Simulation::render()
{
}



int Simulation::inputHandler(sf::Event event, sf::RenderWindow& window) {
	cout << "on recois ! " << endl;
	return 0;
}
