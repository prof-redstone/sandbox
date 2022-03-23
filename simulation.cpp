#include <iostream>
#include <vector>
#include "simulation.h"
#include "main.h"
#include "particle.h"


using namespace sf;
using namespace std;

Simulation::Simulation() {
	image.create(50, 50, Color::Cyan);
	texture.create(50, 50);
	texture.update(image);

	cout << part.position[0] << endl;
}


void Simulation::test()
{
}

void Simulation::updateMove(){
	cout << "updatMove" << endl;
}

void Simulation::move(){
	cout << "move" << endl;
}

void Simulation::render(sf::RenderWindow& window )
{
	window.setTitle("bonsoir !");
	texture.create(window.getSize().x, window.getSize().y);
	image.create(window.getSize().x, window.getSize().y, Color::Cyan);
	texture.update(image);

	//texture dans le sprite
	sprite.setTexture(texture, false);
	window.draw(sprite);


	
}



int Simulation::inputHandler(sf::Event event, sf::RenderWindow& window) {
	cout << "on recois ! " << endl;
	return 0;
}
