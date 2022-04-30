#include <iostream>
#include <vector>
#include "simulation.h"
#include "particle.h"
#include "outils.h"
#include <cstdlib>
#include <ctime>


using namespace sf;
using namespace std;

//if want to add new element add it to inputHandler for button and handplace.
enum type {air,sand,water,stone, wood, salt, saltWater, fire, steam, acid, oil, lava, ice, snow, coal, dirt, bedrock, strangeMatter};
enum MoveType { _Swap, _Replace };

//initialisation of gride and other parameter
Simulation::Simulation(int nbC, int nbR,int  sP) {
	//for SFML render window
	image.create(50, 50, Color::Cyan);
	texture.create(50, 50);
	texture.update(image);
	//to init the random function
	srand((unsigned)this * 41421356237); // for init seed random

	//For inputHandler
	mousePresse = false;
	mouseStillPresse = 0;
	mouseType = sand;


	sizePixel = sP;
	nbCols = nbC/sizePixel;
	nbRows = nbR/sizePixel;
	cout << nbRows;

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
		if (mouseStillPresse == -1) { mouseStillPresse = 1; } //pour attendre 2 tours avant de l'activer, pour etre sur que lastX et Y soit bien du meme type
		if (mouseStillPresse == 0) { mouseStillPresse = -1; }

		Mouse mouse;
		int x = mouse.getPosition(window).x / sizePixel;
		int y = mouse.getPosition(window).y / sizePixel;
		HandPlace(x, y, mouseType);

		mouseLastX = x;
		mouseLastY = y;
	}else {
		mouseStillPresse = 0;
	}


	for (int i = 0; i < nbCols; i++) {
		for (int j = 0; j < nbRows; j++) {
			particleCollect[i][j]->UpdateMove(i, j);
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


	for (int i = 0; i < (nbCols); i++) {
		for (int j = 0; j < (nbRows); j++) {
			Color col;
			if (particleCollect[i][j]->fireConsumTimer < 0) {
				col = particleCollect[i][j]->color;
			}
			else {
				//si en feu
				col = HSLtoRGB((float)((particleCollect[i][j]->fireConsumTimer + (unsigned)particleCollect[i][j]) % 100) /100, 0.05, 0.95, 1.);
			}
			for (int k = 0; k < sizePixel; k++) {
				for (int l = 0; l < sizePixel; l++) {
					image.setPixel(i * sizePixel + k, j * sizePixel + l, col);
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
		}
	}
	if (event.type == Event::MouseButtonReleased) {
		if (event.mouseButton.button == Mouse::Left) {
			mousePresse = false;
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

		if (event.key.code == Keyboard::T){
			mouseType = stone;
		}
		if (event.key.code == Keyboard::S){
			mouseType = sand;
		}
		if (event.key.code == Keyboard::A){
			mouseType = air;
		}
		if (event.key.code == Keyboard::W){
			mouseType = water;
		}
		if (event.key.code == Keyboard::O){
			mouseType = wood;
		}
		if (event.key.code == Keyboard::L){
			mouseType = salt;
		}
		if (event.key.code == Keyboard::F){
			mouseType = fire;
		}
		if (event.key.code == Keyboard::B) {
			mouseType = oil;
		}
		if (event.key.code == Keyboard::C) {
			mouseType = acid;
		}
		if (event.key.code == Keyboard::V) {
			mouseType = lava;
		}
	}
	return "0";
}

//call by InputHandler to place cell in grid when mouse cleck is pressed
void Simulation::HandPlace(int x, int y, int type) {
	const int stroke = 2;

	if (type == sand || type == water || type == salt || type == snow || type == coal || type == dirt || type == acid || type == oil || type == lava || type == ice ) { //all semi solid particle, and liquide
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
	if (type == stone || type == air || type == wood || type == bedrock) {
		if (mouseStillPresse == 1) {
			PlaceBTW(x, y, mouseLastX, mouseLastY, type, stroke);
		}
	}
	if (type == fire) {
		const int nbplace = 70;
		const int dist = 7;
		for (int i = 0; i < nbplace; i++)
		{
			int nX = x + GetRand(-dist, dist);
			int nY = y + GetRand(-dist, dist);
			if (V(nX, nY)) {
				particleCollect[nX][nY]->GetFire();
				
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

void Simulation::PlaceBTW(int matrixX1, int matrixY1, int matrixX2, int matrixY2, int type, int stroke) {
	// If the two points are the same no need to iterate. Just run the provided function
	if (matrixX1 == matrixX2 && matrixY1 == matrixY2) {
		for (int i = 0; i < stroke; i++)
		{
			for (int j = 0; j < stroke; j++) {
				if (V(matrixX1 + i, matrixY1 + j)) {
					AddMove(_Replace, type, -1, matrixX1 + i, matrixY1 + j);
				}
			}
		}
		return;
	}
	//l'algo pour cette fonction n'est pas de moi, mais ça marche. lien :: https://gist.github.com/DavidMcLaughlin208/60e69e698e3858617c322d80a8f174e2
	int xDiff = matrixX1 - matrixX2;
	int yDiff = matrixY1 - matrixY2;
	bool xDiffIsLarger = abs(xDiff) > abs(yDiff);

	int xModifier = xDiff < 0 ? 1 : -1;
	int yModifier = yDiff < 0 ? 1 : -1;

	int longerSideLength = max(abs(xDiff), abs(yDiff));
	int shorterSideLength = min(abs(xDiff), abs(yDiff));
	float slope = (shorterSideLength == 0 || longerSideLength == 0) ? 0 : ((float)(shorterSideLength) / (longerSideLength));

	int shorterSideIncrease;
	for (int i = 1; i <= longerSideLength; i++) {
		shorterSideIncrease = round(i * slope);
		int yIncrease, xIncrease;
		if (xDiffIsLarger) {
			xIncrease = i;
			yIncrease = shorterSideIncrease;
		}
		else {
			yIncrease = i;
			xIncrease = shorterSideIncrease;
		}
		int currentY = matrixY1 + (yIncrease * yModifier);
		int currentX = matrixX1 + (xIncrease * xModifier);
		if (V(currentX, currentY)) {
			for (int i = 0; i < stroke; i++)
			{
				for (int j = 0; j < stroke; j++) {
					if (V(currentX + i, currentY + j)) {
						AddMove(_Replace, type, -1, currentX + i, currentY + j);
					}
				}
			}
		}
	}
}

sf::Color Simulation::HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI)
{
	//hue : 0 : red  1 : yellow  2 : green  3 : cyan  4 : blue  5 : purple  6 : red
	//hue  0 == 6   6 is one cycle rotation
	//saturation [0;1]
	//darkness [0;1]
	//alpha [0;1]

	double red = 0;
	double green = 0;
	double blue = 0;
	double hue = fmod(hueI, 6);

	if (hue >= 0 && hue < 1) {
		red = 255; green = hue * 255; blue = 0;
	}
	else if (hue >= 1 && hue < 2) {
		green = 255; red = 255 - ((hue - 1) * 255); blue = 0;
	}
	else if (hue >= 2 && hue < 3) {
		green = 255; blue = (hue - 2) * 255; red = 0;
	}
	else if (hue >= 3 && hue < 4) {
		blue = 255; green = 255 - ((hue - 3) * 255); red = 0;
	}
	else if (hue >= 4 && hue < 5) {
		blue = 255; red = (hue - 4) * 255; green = 0;
	}
	else if (hue >= 5 && hue < 6) {
		red = 255; blue = 255 - ((hue - 5) * 255); green = 0;
	}

	red = red + (255 - red) * satI;
	green = green + (255 - green) * satI;
	blue = blue + (255 - blue) * satI;

	red = red * darkI;
	green = green * darkI;
	blue = blue * darkI;

	return Color(static_cast<Uint8>(red), static_cast<Uint8>(green), static_cast<Uint8>(blue), static_cast<Uint8>(alphaI * 255));
}