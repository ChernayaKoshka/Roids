#pragma once
#include "vector.h"
#include "drawing.h"
#include "globals.h"

#define DEFAULT_J 20
#define DEFAULT_I 15
#define SCALAR 2
#define SHOT_SCALAR 4

typedef struct tagSpaceship
{
	Vector vector[3];
	DoublePoint origin;
	Vector velocity;
	int score;
}Spaceship;

extern Spaceship ship;

typedef enum tagRotationDirection
{
	LEFT,
	RIGHT
}eRotationDirection;

typedef enum tagMovementDirection
{
	FORWARD,
	BACKWARD,
	STOP
}eMovementDirectionn;

void Ship_Init();
void Ship_ResetVelocity();
BOOL Ship_Rotate(eRotationDirection direction);
BOOL Ship_Accelerate(eMovementDirectionn direction);
void Ship_CheckCollisions();
void Ship_Update();
void Ship_WriteToBuffer();
void Ship_Fire();