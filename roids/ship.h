#pragma once
#include "vector.h"

typedef struct tagSpaceship
{
	Vector vector[3];
	DoublePoint origin;
	Vector velocity;
}Spaceship;