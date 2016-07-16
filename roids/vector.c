#include "vector.h"

float Vector_DotProduct(Vector p1, Vector p2)
{
	return ((p1.i*p2.i) + (p1.j*p2.j));
}

float Vector_GetMagnitude(Vector vector)
{
	return (float)sqrt(vector.i*vector.i + vector.j*vector.j);
}

Vector Vector_Project(Vector vector, Vector onto)
{
	float dp = Vector_DotProduct(vector, onto);
	float magnitude = Vector_GetMagnitude(onto);
	float magsquared = magnitude*magnitude;;
	float scalar = dp / magsquared;

	Vector projection = { 0 };
	projection.i = vector.i*scalar;
	projection.j = vector.j*scalar;
	return projection;
}

Vector Vector_Add(Vector v1, Vector v2)
{
	Vector vector;
	vector.i = v1.i + v2.i;
	vector.j = v1.j + v2.j;
	return vector;
}

Vector Vector_AddRaw(Vector v1, float i, float j)
{
	Vector vector;
	vector.i = v1.i + i;
	vector.j = v1.j + j;
	return vector;
}

//please note that it's v1 - v2.
Vector Vector_Subtract(Vector v1, Vector v2)
{
	Vector vector;
	vector.i = v1.i - v2.i;
	vector.j = v1.j - v2.j;
	return vector;
}

POINT Vector_GetTerminalPoint(Vector v1, POINT offsetFromOrigin)
{
	POINT point;
	point.x = v1.i + offsetFromOrigin.x;
	point.y = v1.j + offsetFromOrigin.y;
	return point;
}

void Vector_Rotate(Vector* v, double degrees)
{
	Vector temp = { v->i,v->j };
	v->i = temp.i * cos(degrees) - temp.j * sin(degrees);
	v->j = temp.i * sin(degrees) + temp.j * cos(degrees);
}

void Vector_Move(Vector* v, double relativeX, double relativeY)
{
	v->i += relativeX;
	v->j += relativeY;
}