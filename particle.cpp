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

enum type { air, sand, water, stone, wood, salt, saltWater, acide };
enum MoveType { _Swap, _Replace };

Particle::Particle(int Itype, vector<int> Ipos, Simulation* sim) {
	type = Itype;
	position = Ipos;
    simulation = sim;

	// to init the random
	srand((unsigned)this* 41421356237 + (unsigned)type * 18748 + (unsigned)position[0]*1754241 + (unsigned)position[1] * 2864856);

	//pour tous 
	flamability = 0;
	fireConsumTimer = -1;
	fireConsumTime = 0;

	if (type == air) {
		color = Color(0, 0, 0);
	}
	if (type == stone) {
		float gray = (double)GetRand(80, 100) ;
		color = Color(gray, gray, gray);
	}
	if (type == wood) {
		int vary = (double)GetRand(-15, 15);
		color = Color(134 + vary, 55 + vary/2 , 11 );
		flamability = 0.01;
		fireConsumTime = 600;
	}
	if (type == sand) {
		friction = 0.2;
		inertieTransfer = 0.5; //entre 0.5 et 0.03
		isFalingTime = 0; //compteur pour savoir pendant combient de temps elle tombe
		moving = false; //false si elle n'a pas bouge entre 2 simulation
		lastposition = position;
		Xvel = 0;
		Yvel = 0;
		color = HSLtoRGB(((double)GetRand(62, 70) / 100),(double)GetRand(20, 30) / 100, (double)GetRand(90, 95) / 100, 1.);
	}
	if (type == salt) {
		WaterToWSaltProb = 0.01;
		friction = 0.2;
		inertieTransfer = 0.5; //entre 0.5 et 0.03
		isFalingTime = 0; //compteur pour savoir pendant combient de temps elle tombe
		moving = false; //false si elle n'a pas bouge entre 2 simulation
		lastposition = position;
		Xvel = 0;
		Yvel = 0;
		color = HSLtoRGB(3.5, (double)GetRand(100, 90) / 100, 0.85, 1.);
	}
	if (type == water) {
		pressure = 0;
		Xvel = 0;
		Yvel = 0;
		flamability = 0.1;
		color = HSLtoRGB(((double)GetRand(375, 385) / 100), 0.5, 0.5, 1.);
	}
	if (type == saltWater) {
		pressure = 0;
		Xvel = 0;
		Yvel = 0;
		flamability = 0.1;
		color = HSLtoRGB(((double)GetRand(360, 375) / 100), 0.5, 0.5, 1.);
	}

}

bool Particle::CanMove(int x, int y, int type) {
	if (simulation->V(x, y)) {
		if (type == sand) {
			int cellT = simulation->particleCollect[x][y]->type;
			//le sable est plus dense et ou lourd que ces materiaux, il peut donc echanger sa place avec pour tomber.
			if (cellT == air) { return true; }
			if (cellT == water) { return true; }
			if (cellT == acide) { return true; }
			if (cellT == saltWater) { return true; }
				
			return false;
		}
		if (type == salt) {
			int cellT = simulation->particleCollect[x][y]->type;
			//le sel est plus dense et ou lourd que ces materiaux, il peut donc echanger sa place avec pour tomber.
			if (cellT == air) { return true; }
			if (cellT == water) { return true; }
			if (cellT == acide) { return true; }
			if (cellT == saltWater) { return true; }

			return false;
		}
		if (type == water) {
			int cellT = simulation->particleCollect[x][y]->type;

			if (cellT == air) { return true; }

			return false;
		}
		if (type == saltWater) {
			int cellT = simulation->particleCollect[x][y]->type;

			if (cellT == air) { return true; }

			return false;
		}
	}
	return false;
}

//renvoie la type de la cellules
int Particle::T(int x, int y) {
	//faire attention il ne reregarde pas si la position est valide; erreur outOfBounds possible.
	return simulation->particleCollect[x][y]->type;
}

int Particle::GetTypeOf(int x, int y) {
	//comme T() mais qui utilise V() avant
	if (simulation->V(x, y)) {
		return T(x, y);
	}
	else {
		return -1;
	}
}

void Particle::UpdateMove(int x, int y) {
	if (type == sand) {
		Sand(x, y);
	}
	else if (type == water) {
		Water(x, y);
	}
	else if (type == salt) {
		Salt(x, y);
	}
	else if (type == saltWater) {
		SaltWater(x, y);
	}

	if (fireConsumTimer > 0) {
		SetNeighborFire(x,y);
		fireConsumTimer--;
	}

	//fini de bruler
	if (fireConsumTimer == 0) {
		if (type == wood) {
			simulation->AddMove(_Replace, air, wood, x, y, x, y);
		}
	}
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
		}
	}

	return; 
}

void Particle::Salt(int x, int y) {
	bool b = CanMove(x, y + 1, salt);
	bool bl = CanMove(x - 1, y + 1, salt);
	bool br = CanMove(x + 1, y + 1, salt);
	bool l = CanMove(x - 1, y, salt);
	bool r = CanMove(x + 1, y, salt);

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
				simulation->AddMove(_Swap, salt, T(x, y + 1), x, y, x, y + 1);
			}
			else if (friction < 1.) {
				if (bl && br) {
					if (GetRand(0, 100) > 50) {
						simulation->AddMove(_Swap, salt, T(x - 1, y + 1), x, y, x - 1, y + 1);
					}
					else {
						simulation->AddMove(_Swap, salt, T(x + 1, y + 1), x, y, x + 1, y + 1);
					}
				}
				else if (bl) {
					simulation->AddMove(_Swap, salt, T(x - 1, y + 1), x, y, x - 1, y + 1);
				}
				else if (br) {
					simulation->AddMove(_Swap, salt, T(x + 1, y + 1), x, y, x + 1, y + 1);
				}

			}
			else {
				if ((GetRand(0, 1001) / 1000.) > (friction - 1.)) {
					if (bl && br) {
						if (GetRand(0, 100) > 50) {
							simulation->AddMove(_Swap, salt, T(x - 1, y + 1), x, y, x - 1, y + 1);
						}
						else {
							simulation->AddMove(_Swap, salt, T(x + 1, y + 1), x, y, x + 1, y + 1);
						}
					}
					else if (bl) {
						simulation->AddMove(_Swap, salt, T(x - 1, y + 1), x, y, x - 1, y + 1);
					}
					else if (br) {
						simulation->AddMove(_Swap, salt, T(x + 1, y + 1), x, y, x + 1, y + 1);
					}
				}
				else {
					moving = false;
				}
			}
			//transmettre moving au voisin, pour la transmission d'inertie.
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

	}
	else if (b) { //si n'a pas bouge depuis la dernière fois, regarde que en dessous en dessous
		simulation->AddMove(_Swap, salt, T(x, y + 1), x, y, x, y + 1);
		TransferInertia(x, y);
		return;
	}
	//si la particle a de l'inertie horizontal et qu'elle n'a pas pu bouger 
	if (Xvel != 0) {
		Xvel = ((GetRand(0, 1001) / 1000.) < friction) ? Xvel / 2 : Xvel;
		if (l || r) {
			TransferInertia(x, y);
			if (Xvel > 0 && r) {
				simulation->AddMove(_Swap, salt, T(x + 1, y), x, y, x + 1, y);
				return;
			}
			if (Xvel < 0 && l) {
				simulation->AddMove(_Swap, salt, T(x - 1, y), x, y, x - 1, y);
				return;
			}
		}
	}

	
	if (GetTypeOf(x, y+1) == water || GetTypeOf(x, y-1) == water || GetTypeOf(x+1, y) == water || GetTypeOf(x-1, y) == water) {
		
		if ( ( (float)GetRand(0, 100) / (float)100) < WaterToWSaltProb) {
			simulation->AddMove(_Replace, saltWater, salt, x, y, x, y);
		}
	}

	return;
}

void Particle::Water(int x, int y) {
	bool b = CanMove(x, y + 1, water);
	bool l = CanMove(x - 1, y, water);
	bool r = CanMove(x + 1, y, water);
	const int lengthPressure = 10;


	if (b || l || r) {
		if (b) {
			simulation->AddMove(_Swap, water, T(x, y + 1), x, y, x, y+1);
			return;
		}
		//calcule de la pression de chaque cote
		pressure = 0;
		for (int i = 0; (i < lengthPressure ); i++)
		{
			if (simulation->ValidType(x + i, y, air)) {
				pressure += 1;
			}
			if (simulation->ValidType(x - i, y, air)) {
				pressure -= 1;
			}
		}
		//et on bouge a droite ou a gauche en fonction du sens de la pression

		if (pressure > 0 && r) {
			simulation->AddMove(_Swap, water, T(x+1, y), x, y, x + 1, y);
		}
		else if (pressure < 0 && l) {
			simulation->AddMove(_Swap, water, T(x-1, y), x, y, x - 1, y);
		}

		return;
	}
}

void Particle::SaltWater(int x, int y) {
	bool b = CanMove(x, y + 1, saltWater);
	bool l = CanMove(x - 1, y, saltWater);
	bool r = CanMove(x + 1, y, saltWater);
	const int lengthPressure = 10;


	if (b || l || r) {
		if (b) {
			simulation->AddMove(_Swap, saltWater, T(x, y + 1), x, y, x, y + 1);
			return;
		}
		//calcule de la pression de chaque cote
		pressure = 0;
		for (int i = 0; (i < lengthPressure); i++)
		{
			if (simulation->ValidType(x + i, y, air)) {
				pressure += 1;
			}
			if (simulation->ValidType(x - i, y, air)) {
				pressure -= 1;
			}
		}
		//et on bouge a droite ou a gauche en fonction du sens de la pression

		if (pressure > 0 && r) {
			simulation->AddMove(_Swap, saltWater, T(x + 1, y), x, y, x + 1, y);
		}
		else if (pressure < 0 && l) {
			simulation->AddMove(_Swap, saltWater, T(x - 1, y), x, y, x - 1, y);
		}

		return;
	}


	if (GetTypeOf(x, y + 1) == water || GetTypeOf(x, y - 1) == water || GetTypeOf(x + 1, y) == water || GetTypeOf(x - 1, y) == water) {

		if (((float)GetRand(0, 100) / (float)100) < 0.5) {
			const int ran = GetRand(0, 4);
			if (ran == 0 && simulation->V(x + 1, y)) {
				simulation->AddMove(_Swap, saltWater, water, x, y, x+1, y);
			}
			if (ran == 1 && simulation->V(x - 1, y)) {
				simulation->AddMove(_Swap, saltWater, water, x, y, x - 1, y);
			}
			if (ran == 2 && simulation->V(x, y+1)) {
				simulation->AddMove(_Swap, saltWater, water, x, y, x, y+1);
			}
			if (ran == 3 && simulation->V(x, y-1)) {
				simulation->AddMove(_Swap, saltWater, water, x, y, x, y-1);
			}

		}
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

void Particle::SetNeighborFire(int x, int y) {
	int a = GetRand(-5, 5);
	int b = GetRand(-5, 5);

	if (x != 0 || y != 0) {
		if (simulation->V(x + a, y + b)) {
			if (simulation->particleCollect[x + a][y + b]->flamability != 0) {
				simulation->particleCollect[x + a][y + b]->GetFire();
			}
		}
	}
}

void Particle::GetFire() {
	if (fireConsumTimer <= 0) {
		if (GetRand(0, 1000) / 1000 < flamability) {
			fireConsumTimer = fireConsumTime + GetRand(-fireConsumTime / 4, fireConsumTime / 4);
		}
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