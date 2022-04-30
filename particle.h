#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;



class Simulation;

class Particle {
public:
	//for multi type :
	int type;
	Simulation* simulation;
	vector<int> position;
	vector<int> lastposition;
	Color color;
	int Yvel;
	int Xvel;
	float flamability;
	int fireConsumTimer;
	int fireConsumTime;
	float corrosion;//prob to an element to be attack by acid

	//for movable solid like sand :
	float friction;
	float inertieTransfer;
	int isFalingTime;
	bool moving;

	//for liquide:
	int pressure;

	//for water
	float WaterToWSaltProb; //prob for water next to salt, to transform in to salt water

	//for steam
	float steamCondensationProb;

	//acid
	float acidCorrosionProb;

	//lava
	float viscosity;
	


	Particle(int Itype, vector<int> Ipos, Simulation* sim);
	bool CanMove(int x, int y, int type);
	int T(int x, int y);
	int GetTypeOf(int x, int y);
	void UpdateMove(int x, int y);

	void Sand(int x, int y);
	void Salt(int x, int y);
	void Water(int x, int y);
	void SaltWater(int x, int y);
	void Steam(int x, int y);
	void Oil(int x, int y);
	void Acid(int x, int y);
	void Lava(int x, int y);
	void Coal(int x, int y);
	void Snow(int x, int y);

	void TransferInertia(int x, int y);
	void RecivedInertia();
	void SetNeighborFire(int x, int y);
	void GetFire();
	int GetRand(int a, int b);
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};