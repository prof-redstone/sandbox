#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "main.h"
#include "particle.h"

using namespace std;
using namespace sf;



enum type {
	air,
	sand,
	water,
	stone,
	acide
};


Particle::Particle(int Itype, vector<int> Ipos) {
	type = Itype;
	position = Ipos;

	//cout << this << endl;
	srand((unsigned)this* 41421356237);
	//cout << GetRand(5,10);

	if (type == air) {
		color = Color(50, 50, 50);
	}
	else if (type == sand) {
		color = HSLtoRGB(((double)GetRand(62, 70) / 100),(double)GetRand(20, 30) / 100, (double)GetRand(90, 95) / 100, 1.);
	}
}

int Particle::GetRand(int a, int b) {
    if (b - a == 0) { return a; }
	return (rand() % (b - a)) + a;
}

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