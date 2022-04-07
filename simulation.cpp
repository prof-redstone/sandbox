#include <iostream>
#include <vector>
#include "simulation.h"
#include "particle.h"
#include "outils.h"
#include <cstdlib>
#include <ctime>


using namespace sf;
using namespace std;


enum type {air,sand,water,stone,acide};
enum moveType { _swap, _replace };


Simulation::Simulation() {
	srand((unsigned)time(0));
	image.create(50, 50, Color::Cyan);
	texture.create(50, 50);
	texture.update(image);

	srand((unsigned)this); // for init seed random

	nbCols = 100;
	nbRows = 100;

	particleCollect = vector<vector<Particle*>>(nbCols, vector<Particle*>(nbRows, nullptr));

	for (int i = 0; i < nbCols; i++){
		for (int j = 0; j < nbRows; j++){
			particleCollect[i][j] = new Particle(0, { i,j });
		}
	}

	replace(sand, 10, 10);
}

int Simulation::getRand(int a, int b) {
	return (rand() % (b - a)) + a;
}

void Simulation::replace(int type, int a , int b ){
	delete particleCollect[a][b];
	particleCollect[a][b] = new Particle(type, { a,b });
}

void Simulation::swap(int x1, int y1, int x2, int y2) {
	Particle* temp = particleCollect[x1][y1];
	particleCollect[x1][y1] = particleCollect[x2][y2];
	particleCollect[x2][y2] = temp;
}


void Simulation::updateMove(){
	for (int i = 0; i < nbCols; i++){
		for (int j = 0; j < nbRows; j++){
			if (particleCollect[i][j]->type == sand) { Sand(i, j); }
		}
	}
}

void Simulation::move(){
	for (int i = 0; i < moves.size(); i++){
		CheckAndMove(moves[i][0], moves[i][1], moves[i][2], moves[i][3], moves[i][4], moves[i][5], moves[i][6]);
	}
	moves = {};
	/*while (moves.size() > 0) {
		int i = getRand(0, moves.size());
		CheckAndMove(moves[i]);
		moves.erase(moves.begin() + i);
	}*/
}

void Simulation::CheckAndMove(int Tmove, int T1, int T2, int X1, int Y1, int X2, int Y2) {
	if (Tmove == _swap){
		if (particleCollect[X1][Y1]->type == T1 && particleCollect[X2][Y2]->type == T2){
			swap(X1, Y1, X2, Y2);
		}
	}
	else if (Tmove == _replace) {
		if (particleCollect[X1][Y1]->type == T2 || T2 == -1) {
			replace(T1, X1, Y1);
		}
	}
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

String Simulation::inputHandler(sf::Event event, sf::RenderWindow& window) {
	if (event.type == Event::MouseButtonPressed) {
		if (event.mouseButton.button == Mouse::Left) {
			Mouse mouse;
			int x = mouse.getPosition(window).x / (window.getSize().x / particleCollect.size()) ;
			int y = mouse.getPosition(window).y / (window.getSize().y / particleCollect.size()) ;
			//replace(sand, x, y);
			HandPlace(x, y, sand);
		}
	}
	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::Up)//incredase limit
		{
			return "FPS UP";
		}
		if (event.key.code == Keyboard::Down)//decrease limit
		{
			return "FPS DOWN";
		}
	}
	return "0";
}

bool Simulation::V(int x, int y) {
	return x >= 0 && x < Simulation::nbCols && y >= 0 && y < Simulation::nbRows;
}

void Simulation::AddMove(int Tmove, int T1,int T2,int X1, int Y1, int X2 = -1, int Y2 = -1) {
	if (Tmove == _swap) { //swap two cells
		moves.push_back({ Tmove, T1, T2, X1, Y1, X2, Y2 });
	}
	else if (Tmove == _replace) { //replace one cell, change the type or contente of cell
		moves.push_back({ Tmove, T1, T2, X1, Y1, X2, Y2 });
 	}
}

bool Simulation::CheckMove(int x , int y, int t) {
	if (V(x, y)) {
		return particleCollect[x][y]->type == t;
	}
	return false;
}

void Simulation::Sand (int x, int y){
	bool b = Simulation::CheckMove(x, y + 1, air);
	bool bWater = Simulation::CheckMove(x, y + 1, water);

	bool bl = Simulation::CheckMove(x - 1, y + 1, air);
	bool br = Simulation::CheckMove(x + 1, y + 1, air);

	if (b) {
		//Simulation::AddMove(x, y, x, y + 1, sand, air, air);
		Simulation::AddMove(_swap, sand, air, x, y, x, y + 1);
	}
	/*else if (bWater) {
		Simulation::AddMove(x, y, x, y + 1, sand, water, water);
	}*/
	else if (bl && br) {
		if (true) {
			Simulation::AddMove(_swap, sand, air, x, y, x - 1, y + 1);
		}
		else {
			Simulation::AddMove(_swap, sand, air, x, y, x + 1, y + 1);
		}
	}
	else if (bl) {
		Simulation::AddMove(_swap, sand, air, x, y, x - 1, y + 1);
	}
	else if (br) {
		Simulation::AddMove(_swap, sand, air, x, y, x + 1, y + 1);
	}
}

void Simulation::HandPlace(int x, int y, int type) {
	const int nbplace = 15;
	const int dist = 4;
	for (int i = 0; i < nbplace; i++)
	{
		int nX = x + getRand(-dist, dist);
		int nY = y + getRand(-dist, dist);
		if (V(nX, nY)) {
			AddMove(_replace, type, -1, nX, nY);
		}
	}
}