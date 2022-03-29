
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib> //pour random
#include <ctime> //pour random
#include "outils.h"

Rand::Rand()
{
	srand((unsigned)time(0));
}

int Rand::get(int a, int b) {

	int i = (rand() % (a - b)) + a;
	return i;
}
