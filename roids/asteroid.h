#pragma once
#include <Windows.h>
#include "vector.h"
#include "drawing.h"
#include "screen.h"

#define ASTEROID_WIDTH 20
#define ASTEROID_HEIGHT 20

#define ASTEROID_MAX_START_VELOCITY 2.0
#define ASTEROID_MIN_START_VELOCITY 0.0

typedef struct tagAsteroid
{
	char id;
	RECT asteroid;
	DoublePoint origin;
	Vector velocity;
}Asteroid;

extern Asteroid roids[5];

BOOL Asteroid_Init();
RECT Asteroid_AdjustRectForOrigin(Asteroid asteroid);
void Asteroid_Update();
void Asteroid_WriteToBuffer();