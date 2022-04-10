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
		friction = 1.1;
		isFalingTime = 0;
		moving = false;
		lastposition = position;
		Xvel = 0;
		Yvel = 0;
		color = HSLtoRGB(((double)GetRand(62, 70) / 100),(double)GetRand(20, 30) / 100, (double)GetRand(90, 95) / 100, 1.);
	}
}


void Particle::Sand(int x, int y) {
	bool b = simulation->ValidType(x, y + 1, air); 
	bool bWater = simulation->ValidType(x, y + 1, water);
	bool bl = simulation->ValidType(x - 1, y + 1, air);
	bool br = simulation->ValidType(x + 1, y + 1, air);
	bool l = simulation->ValidType(x - 1, y, air);
	bool r = simulation->ValidType(x + 1, y, air);

	if (position[0] != x || position[1] != y) {//mettre à jour la position et l'ancienne si elle a change
		lastposition = { position[0], position[1] };
		position = { x,y };

		moving = true;
	}


	if (moving) { //si a bouge juste avant ou voisin lui permet de bouger

		if (b || bl || br) {
			isFalingTime++;
			moving = true;
			if (b) {
				simulation->AddMove(_Swap, sand, air, x, y, x, y + 1);
			}
			else if(friction < 1.){
				if (bl && br) {
					if (GetRand(0, 100) > 50) {
						simulation->AddMove(_Swap, sand, air, x, y, x - 1, y + 1);
					}
					else {
						simulation->AddMove(_Swap, sand, air, x, y, x + 1, y + 1);
					}
				}
				else if (bl) {
					simulation->AddMove(_Swap, sand, air, x, y, x - 1, y + 1);
				}
				else if (br) {
					simulation->AddMove(_Swap, sand, air, x, y, x + 1, y + 1);
				}

			}else {
				if ((GetRand(0, 1001) / 1000.) > (friction - 1.)) {
					if (bl && br) {
						if (GetRand(0, 100) > 50) {
							simulation->AddMove(_Swap, sand, air, x, y, x - 1, y + 1);
						}
						else {
							simulation->AddMove(_Swap, sand, air, x, y, x + 1, y + 1);
						}
					}
					else if (bl) {
						simulation->AddMove(_Swap, sand, air, x, y, x - 1, y + 1);
					}
					else if (br) {
						simulation->AddMove(_Swap, sand, air, x, y, x + 1, y + 1);
					}
				}
				else {
					moving = false;
				}
			}
			//transmettre moving au voisin
			return;
		}

		//si Inertie horizontal et peut aller gauche ou droite
		if ((l || r)) {
			//Si inertie V convertire en inertie H
			Yvel = ((isFalingTime / 10) > 2) ? 4 : isFalingTime / 10;
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

	}else if (b) { //si toujours à la meme postion regarde en dessous
		simulation->AddMove(_Swap, sand, air, x, y, x, y + 1);
		return;
	}

	//si la particle a de l'enertie horizontal et qu'elle n'a pu bouger 
	if (Xvel != 0) {
		Xvel = ((GetRand(0, 1001) / 1000.) < friction) ? Xvel / 2 : Xvel;
		if (l || r) {
			if (Xvel > 0 && r) {
				simulation->AddMove(_Swap, sand, air, x, y, x + 1, y);
				return;
			}
			if (Xvel < 0 && l) {
				simulation->AddMove(_Swap, sand, air, x, y, x - 1, y);
				return;
			}
		}else {
			Xvel = 0;
		}
	}

	//-----------------------fin----------------------------
	return; 


	if (b) {//peut tomber vers le bas
		isFalingTime++;
		Yvel = ((isFalingTime / 10) > 2) ? 4 : isFalingTime / 10;
		moving = true;
		simulation->AddMove(_Swap, sand, air, x, y, x, y + 1);
		return;
		//mettre le freeFaling au voisin
	}
	else { //peut pas tomber vers le bas

		//si inertie horizontal on ajoute la friction
		if (Xvel != 0) { 
			Xvel = ((GetRand(0,1001)/1000.) < friction) ? Xvel/2 : Xvel;
		}
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

	if (Xvel != 0) {//si inertie horizontal on bouge
		if (Xvel > 0 && r) {
			simulation->AddMove(_Swap, sand, air, x, y, x + 1, y);
			return;
		}
		if (Xvel < 0 && l) {
			simulation->AddMove(_Swap, sand, air, x, y, x - 1, y);
			return;
		}
	}


	if (moving) { //si free falling on check des case en plus 
		if (r) {
			simulation->AddMove(_Swap, sand, air, x, y, x + 1, y);
			return;
		}
		if (l) {
			simulation->AddMove(_Swap, sand, air, x, y, x - 1, y);
			return;
		}
	}


	/*else if (bWater) {
		Simulation::AddMove(x, y, x, y + 1, sand, water, water);
	}else if (bl && br) {
		if (GetRand(0, 100) > 50) {
			simulation->AddMove(_Swap, sand, air, x, y, x - 1, y + 1);
		}
		else {
			simulation->AddMove(_Swap, sand, air, x, y, x + 1, y + 1);
		}
	}
	else if (bl) {
		simulation->AddMove(_Swap, sand, air, x, y, x - 1, y + 1);
	}
	else if (br) {
		simulation->AddMove(_Swap, sand, air, x, y, x + 1, y + 1);
	}*/
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