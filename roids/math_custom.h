#pragma once

#include <Windows.h>
#include <math.h>
#include "vector.h"

#define M_PI 3.141592653589793238462643383279502884197169399375105820974

typedef struct tagSlope
{
	int dx;
	int dy;
}Slope;

int Greatest(int num, int num2);
int Least(int num, int num2);
int Difference(int num, int num2);
int Abs(int val);
double getRandomDoubleInRange(double min, double max);
int GetColor(double* angle);
int GetRandRGB();
void specialPlot(int* screen, int screenWidth, int x, int y, int size, int color);

Slope getSlope(POINT p1, POINT p2);
double getDistance(POINT p1, POINT p2);

DoublePoint* convertRectToPoints(RECT rect);

int roundTo(int numToRound, int multiple);
int roundUpTo(int numToRound, int multiple);
int roundDownTo(int numToRound, int multiple);

BOOL lineIntersectsRect(DoublePoint start, DoublePoint end, RECT rect);
BOOL doRectanglesOverlap(RECT r1, RECT r2);