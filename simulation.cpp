#include <iostream>
#include <vector>
#include "simulation.h"
#include "particle.h"
#include "outils.h"


using namespace sf;
using namespace std;

enum type {air,sand,water,stone,acide};

Simulation::Simulation() {
	image.create(50, 50, Color::Cyan);
	texture.create(50, 50);
	texture.update(image);

	nbCols = 100;
	nbRows = 100;

	particleCollect = vector<vector<Particle*>>(nbCols, vector<Particle*>(nbRows, nullptr));

	for (int i = 0; i < nbCols; i++){
		for (int j = 0; j < nbRows; j++){
			particleCollect[i][j] = new Particle(air, { i,j }, *particleCollect);
		}
	}

	replace(sand, 10, 10);
}


void Simulation::replace(int type, int a , int b ){
	delete particleCollect[a][b];
	particleCollect[a][b] = new Particle(type, { a,b }, *particleCollect);
}

void Simulation::updateMove(){
	swap(particleCollect[10][10], particleCollect[10][11]);
	for (int i = 0; i < nbCols; i++)
	{
		for (int j = 0; j < nbRows; j++)
		{
			particleCollect[i][j]->checkmove();
		}
	}
}

void Simulation::move(){
	cout << "move" << endl;
}

void Simulation::render(sf::RenderWindow& window )
{
	texture.create(window.getSize().x, window.getSize().y);
	image.create(window.getSize().x, window.getSize().y, Color::Black);
	int sizePiX = window.getSize().x / particleCollect.size();
	int sizePiY = window.getSize().y / particleCollect[0].size();

	
	for (int i = 0; i < (particleCollect.size()); i++){
		for (int j = 0; j < (particleCollect[0].size()); j++){
			Color col = particleCollect[i][j]->color;
			for (int k = 0; k < sizePiX; k++){
				for (int l = 0; l < sizePiY; l++){
					image.setPixel(i*sizePiX + k,j*sizePiY + l, col);
				}
			}
		}
	}

	//texture dans le sprite
	texture.update(image);
	sprite.setTexture(texture, false);
	window.draw(sprite);
}



int Simulation::inputHandler(sf::Event event, sf::RenderWindow& window) {
	cout << "on recois ! " << endl;
	return 0;
}

void swap(int* i, int* j)
{
	int temp;

	temp = *i;
	*i = *j;
	*j = temp;
}


class Move {

};