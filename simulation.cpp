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
enum MoveType { _Swap, _Replace };

//initialisation of gride and other parameter
Simulation::Simulation() {
	//for SFML render window
	image.create(50, 50, Color::Cyan);
	texture.create(50, 50);
	texture.update(image);
	//to init the random function
	srand((unsigned)this * 41421356237); // for init seed random

	//For inputHandler
	mousePresse = false;
	mouseType = sand;

	nbCols = 150;
	nbRows = 150;
	particleCollect = vector<vector<Particle*>>(nbCols, vector<Particle*>(nbRows, nullptr));
	 
	for (int i = 0; i < nbCols; i++){
		for (int j = 0; j < nbRows; j++){
			particleCollect[i][j] = new Particle(0, { i,j }, this);
		}
	}

}

//first function, who loop each frame to check at each cell to update it, and Move it, place all Move proposition in queu.
void Simulation::UpdateMove(sf::RenderWindow& window) {
	if (mousePresse) {
		Mouse mouse;
		int x = mouse.getPosition(window).x / (window.getSize().x / particleCollect.size());
		int y = mouse.getPosition(window).y / (window.getSize().y / particleCollect.size());
		HandPlace(x, y, mouseType);
	}


	for (int i = 0; i < nbCols; i++) {
		for (int j = 0; j < nbRows; j++) {
			if (particleCollect[i][j]->type == sand) { 
				particleCollect[i][j]->Sand(i, j);
			}
			if (particleCollect[i][j]->type == water) {
				particleCollect[i][j]->Water(i, j);
			}
		}
	}
}

//second function, who loop each frame to apply each Move proposition in queu.
void Simulation::Move() {
	vector<int> indexArr = RArray(Moves.size());
	for (int i = 0; i < Moves.size(); i++) {
		int a = indexArr[i]; //to get random order Move application to avoid pattern
		CheckAndMove(Moves[a][0], Moves[a][1], Moves[a][2], Moves[a][3], Moves[a][4], Moves[a][5], Moves[a][6]);
	}
	//reset list
	Moves = {};
}

//To Render all cell in windows using SFML
void Simulation::Render(sf::RenderWindow& window)
{
	texture.create(window.getSize().x, window.getSize().y);
	image.create(window.getSize().x, window.getSize().y, Color::Black);
	int sizePiX = window.getSize().x / particleCollect.size();
	int sizePiY = window.getSize().y / particleCollect[0].size();


	for (int i = 0; i < (particleCollect.size()); i++) {
		for (int j = 0; j < (particleCollect[0].size()); j++) {
			Color col = particleCollect[i][j]->color;
			for (int k = 0; k < sizePiX; k++) {
				for (int l = 0; l < sizePiY; l++) {
					image.setPixel(i * sizePiX + k, j * sizePiY + l, col);
				}
			}
		}
	}

	//texture dans le sprite
	texture.update(image);
	sprite.setTexture(texture, false);
	window.draw(sprite);
}

//For each Move proposition, check for no probleme, no duplication, and if each cell are ok, then Move it
void Simulation::CheckAndMove(int TMove, int T1, int T2, int X1, int Y1, int X2, int Y2) {
	if (TMove == _Swap) {
		if (particleCollect[X1][Y1]->type == T1 && particleCollect[X2][Y2]->type == T2) {
			Swap(X1, Y1, X2, Y2);
		}
	}
	else if (TMove == _Replace) {
		if (particleCollect[X1][Y1]->type == T2 || T2 == -1) {
			Replace(T1, X1, Y1);
		}
	}
}

//each cell add a Move to queu to execute them later
void Simulation::AddMove(int TMove, int T1, int T2, int X1, int Y1, int X2 = -1, int Y2 = -1) {
	if (TMove == _Swap) { //Swap two cells
		Moves.push_back({ TMove, T1, T2, X1, Y1, X2, Y2 });
	}
	else if (TMove == _Replace) { //Replace one cell, change the type or contente of cell
		Moves.push_back({ TMove, T1, T2, X1, Y1, X2, Y2 });
	}
}

//Replace specific cell in grid to another
void Simulation::Replace(int type, int a , int b ){
	delete particleCollect[a][b];
	particleCollect[a][b] = new Particle(type, { a,b }, this);
}

//Swap to cells in grid
void Simulation::Swap(int x1, int y1, int x2, int y2) {
	Particle* temp = particleCollect[x1][y1];
	particleCollect[x1][y1] = particleCollect[x2][y2];
	particleCollect[x2][y2] = temp;
}

//Get all input from mouse and keyboard, and place cell, change type cell.
String Simulation::InputHandler(sf::Event event, sf::RenderWindow& window) {
	if (event.type == Event::MouseButtonPressed) {
		if (event.mouseButton.button == Mouse::Left) {
			mousePresse = true;
			cout << "pressed" << endl;
		}
	}
	if (event.type == Event::MouseButtonReleased) {
		if (event.mouseButton.button == Mouse::Left) {
			mousePresse = false;
			cout << "relaesed" << endl;
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
		if (event.key.code == Keyboard::T)//decrease limit
		{
			mouseType = stone;
		}
		if (event.key.code == Keyboard::S)//decrease limit
		{
			mouseType = sand;
		}
		if (event.key.code == Keyboard::A)//decrease limit
		{
			mouseType = air;
		}
		if (event.key.code == Keyboard::W)//decrease limit
		{
			mouseType = water;
		}
	}
	return "0";
}

//call by InputHandler to place cell in grid when mouse cleck is pressed
void Simulation::HandPlace(int x, int y, int type) {
	if (type == sand || type == water) { //all semi solid particle
		const int nbplace = 15;
		const int dist = 10;
		for (int i = 0; i < nbplace; i++)
		{
			int nX = x + GetRand(-dist, dist);
			int nY = y + GetRand(-dist, dist);
			if (V(nX, nY)) {
				AddMove(_Replace, type, -1, nX, nY);
			}
		}
	}
	if (type == stone || type == air) {
		const int stroke = 2;
		for (int i = 0; i < stroke; i++)
		{
			for (int j = 0; j < stroke; j++) {
				if (V(x + i, y + j)) {
					AddMove(_Replace, type, -1, x+i, y+j);
				}
			}
		}
	}
}
//check if coordonate are in gride array to avoid bounds exception and crash.
bool Simulation::V(int x, int y) {
	return x >= 0 && x < Simulation::nbCols && y >= 0 && y < Simulation::nbRows;
}

//return type cell at specifique coordinate
bool Simulation::ValidType(int x , int y, int t) {
	if (V(x, y)) {
		return particleCollect[x][y]->type == t;
	}
	return false;
}

//generate an array of int in random order to avoid pattern on Move application to get more organique simulation
vector<int> Simulation::RArray(const int& max) {
	vector<int> a;
	for (int i = 0; i < max; i++){
		a.push_back(i);
	}
	for (int i = 0; i < max; i++)
	{
		int b = GetRand(0, max-1);
		int c = GetRand(0, max-1);
		int d = a[b];
		a[b] = a[c];
		a[c] = d;
	}
	return a;
}

//to get a random Interger
int Simulation::GetRand(int a, int b) {
	if (b - a == 0) { return a; }
	return (rand() % (b - a)) + a;
}