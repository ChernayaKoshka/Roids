#pragma once
#include <Windows.h>
#include <math.h>

typedef struct tagVector
{
	double i;
	double j;
}Vector;

typedef struct tagDoublePoint
{
	double x;
	double y;
}DoublePoint;

float Vector_DotProduct(Vector p1, Vector p2);
float Vector_GetMagnitude(Vector vector);
Vector Vector_Project(Vector vector, Vector onto);
Vector Vector_Add(Vector v1, Vector v2);
Vector Vector_AddRaw(Vector v1, float i, float j);
Vector Vector_Subtract(Vector v1, Vector v2);
POINT Vector_GetTerminalPoint(Vector v1, POINT offsetFromOrigin);
void Vector_Rotate(Vector* v, double deltaAngle);
void Vector_Move(Vector* v, double relativeX, double relativeY);