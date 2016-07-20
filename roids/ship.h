#pragma once
#include "vector.h"
#include "drawing.h"
#include "globals.h"

#define DEFAULT_J 20
#define DEFAULT_I 15
#define SCALAR 2
#define SHOT_LENGTH_SCALAR 4
#define SHIP_VELOCITY_LIMITING_SCALAR 20
#define SHIP_BUMP_VELOCCITY_REDUCTION_SCALAR 2
#define SHIP_ROTATION_LIMITER 10

typedef struct tagSpaceship
{
	Vector vector[3];
	DoublePoint origin;
	Vector velocity;
	int score;
}Spaceship;

extern Spaceship ship;

typedef enum tagShipPoint
{
	NOSE,
	LEFT, //starting position, doesn't change
	RIGHT //ditto
}ShipPoint;

typedef enum tagRotationDirection
{
	ROTATE_LEFT,
	ROTATE_RIGHT
}RotationDirection;

typedef enum tagMovementDirection
{
	FORWARD,
	BACKWARD,
	STOP
}MovementDirectionn;

void Ship_Init();
void Ship_ResetVelocity();
BOOL Ship_Rotate(RotationDirection direction);
BOOL Ship_Accelerate(MovementDirectionn direction);
void Ship_CheckCollisions();
void Ship_Update();
void Ship_WriteToBuffer();
void Ship_Fire();