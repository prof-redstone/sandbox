#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>     /* abs */
#include "main.h"
#include "particle.h"
#include "simulation.h"

using namespace std;
using namespace sf;

enum type { air, sand, water, stone, acide };
enum MoveType { _Swap, _Replace };

Particle::Particle(int Itype, vector<int> Ipos, Simulation* sim) {
	type = Itype;
	position = Ipos;
    simulation = sim;

	// to init the random
	srand((unsigned)this* 41421356237 + (unsigned)type * 18748 + (unsigned)position[0]*1754241 + (unsigned)position[1] * 2864856);


	if (type == air) {
		friction = 0;
		color = Color(50, 50, 50);
	}
	else if (type == sand) {
		friction = 0.2;
		inertieTransfer = 0.5; //entre 0.5 et 0.03
		isFalingTime = 0; //compteur pour savoir pendant combient de temps elle tombe
		moving = false; //false si elle n'a pas bouge entre 2 simulation
		lastposition = position;
		Xvel = 0;
		Yvel = 0;
		color = HSLtoRGB(((double)GetRand(62, 70) / 100),(double)GetRand(20, 30) / 100, (double)GetRand(90, 95) / 100, 1.);
	}
	else if (type == water) {
		pressure = 0;
		Xvel = 0;
		Yvel = 0;
		color = HSLtoRGB(((double)GetRand(375, 385) / 100), 0.5, 0.5, 1.);
	}
}

bool Particle::CanMove(int x, int y, int type) {
	if (simulation->V(x, y)) {
		if (type == sand) {
			int cellT = simulation->particleCollect[x][y]->type;
			if (cellT == air) { return true; }
			if (cellT == water) { return true; }
			if (cellT == acide) { return true; }
				
			return false;
		}
	}
	return false;
}

int Particle::T(int x, int y) {
	//faire attention il ne reregarde pas si la position est valide; erreur outOfBounds possible.
	return simulation->particleCollect[x][y]->type;
}

void Particle::Sand(int x, int y) {
	bool b = CanMove(x, y + 1, sand);
	bool bl = CanMove(x - 1, y + 1, sand);
	bool br = CanMove(x + 1, y + 1, sand);
	bool l = CanMove(x - 1, y, sand);
	bool r = CanMove(x + 1, y, sand);

	if (position[0] != x || position[1] != y) {//mettre à jour la position et l'ancienne si elle a change
		lastposition = { position[0], position[1] };
		position = { x,y };
		moving = true;
	}

	if (moving) { //si a bouge juste avant ou voisin lui permet de bouger

		if (b || bl || br) {
			isFalingTime++;
			Yvel = ((isFalingTime / 10) > 4) ? 4 : isFalingTime / 10; //plus elle tombe depuis longtemps plus elle a d'inertie
			moving = true;
			if (b) {
				simulation->AddMove(_Swap, sand, T(x,y+1), x, y, x, y + 1);
			}else if(friction < 1.){
				if (bl && br) {
					if (GetRand(0, 100) > 50) {
						simulation->AddMove(_Swap, sand, T(x-1, y + 1), x, y, x - 1, y + 1);
					}else {
						simulation->AddMove(_Swap, sand, T(x+1, y + 1), x, y, x + 1, y + 1);
					}
				}else if (bl) {
					simulation->AddMove(_Swap, sand, T(x-1, y + 1), x, y, x - 1, y + 1);
				}else if (br) {
					simulation->AddMove(_Swap, sand, T(x+1, y + 1), x, y, x + 1, y + 1);
				}

			}else {
				if ((GetRand(0, 1001) / 1000.) > (friction - 1.)) {
					if (bl && br) {
						if (GetRand(0, 100) > 50) {
							simulation->AddMove(_Swap, sand, T(x - 1, y + 1), x, y, x - 1, y + 1);
						}else {
							simulation->AddMove(_Swap, sand, T(x + 1, y + 1), x, y, x + 1, y + 1);
						}
					}else if (bl) {
						simulation->AddMove(_Swap, sand, T(x - 1, y + 1), x, y, x - 1, y + 1);
					}else if (br) {
						simulation->AddMove(_Swap, sand, T(x + 1, y + 1), x, y, x + 1, y + 1);
					}
				}else {
					moving = false;
				}
			}
			//transmettre moving au voisin
			TransferInertia(x, y);
			return;
		}

		moving = false; // n'a donc pas pu bouger

		//si Inertie horizontal et peut aller gauche ou droite
		if ((l || r)) {
			//Si inertie Vertical, convertire en inertie H, car touche le sol
			isFalingTime = 0;
			//convertion de l'energie vertical en horizontal
			if (Yvel != 0) {
				if (l && r) {
					Xvel = (GetRand(0, 2) * 2 - 1) * Yvel;
				}
				else if (l) {
					Xvel = -1 * Yvel;
				}
				else if (r) {
					Xvel = 1 * Yvel;
				}
				Yvel = 0;
			}
		}

	}else if (b) { //si n'a pas bouge depuis la dernière fois, regarde que en dessous en dessous
		simulation->AddMove(_Swap, sand, T(x, y + 1), x, y, x, y + 1);
		TransferInertia(x, y);
		return;
	}
	//si la particle a de l'inertie horizontal et qu'elle n'a pas pu bouger 
	if (Xvel != 0) {
		Xvel = ((GetRand(0, 1001) / 1000.) < friction) ? Xvel / 2 : Xvel;
		if (l || r) {
			TransferInertia(x, y);
			if (Xvel > 0 && r) {
				simulation->AddMove(_Swap, sand, T(x + 1, y), x, y, x + 1, y);
				return;
			}
			if (Xvel < 0 && l) {
				simulation->AddMove(_Swap, sand, T(x - 1, y), x, y, x - 1, y);
				return;
			}
		}else {
			//Xvel = 0;
		}
	}

	return; 
}

void Particle::Water(int x, int y) {
	bool b = simulation->ValidType(x, y + 1, air);
	bool l = simulation->ValidType(x - 1, y, air);
	bool r = simulation->ValidType(x + 1, y, air);
	const int lengthPressure = 4;


	if (b || l || r) {
		if (b) {
			simulation->AddMove(_Swap, water, air, x, y, x, y+1);
			return;
		}
		//calcule de la pression de chaque cote
		pressure = 0;
		for (int i = 0; i < lengthPressure; i++)
		{
			if (simulation->ValidType(x + i, y, air)) {
				pressure++;
			}
			if (simulation->ValidType(x - i, y, air)) {
				pressure--;
			}
		}
		//et on bouge a droite ou a gauche en fonction du sens de la pression

		if (pressure > 0 && r) {
			simulation->AddMove(_Swap, water, air, x, y, x + 1, y);
		}
		else if (pressure < 0 && l) {
			simulation->AddMove(_Swap, water, air, x, y, x - 1, y);
		}

		return;
	}
}

void Particle::TransferInertia(int x, int y) {
	if (simulation->V(x + 1, y)) {
		simulation->particleCollect[x + 1][y]->RecivedInertia();
	}
	if (simulation->V(x - 1, y)) {
		simulation->particleCollect[x - 1][y]->RecivedInertia();
	}
	if (simulation->V(x, y + 1)) {
		simulation->particleCollect[x][y + 1]->RecivedInertia();
	}
	if (simulation->V(x, y - 1)) {
		simulation->particleCollect[x][y - 1]->RecivedInertia();
	}
}


void Particle::RecivedInertia() {
	if ((GetRand(0, 1001) / 1000.) < inertieTransfer) {
		//Xvel = 5;
		Xvel = (GetRand(0, 1001) / 1000. > 0.5) ? 4 : -4;
	}
}

//to get a random Interger
int Particle::GetRand(int a, int b) {
    if (b - a == 0) { return a; }
	return (rand() % (b - a)) + a;
}

//to converte HSL color mode to RGBA
sf::Color Particle::HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI)
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