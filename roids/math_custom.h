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

int MC_Greatest(int num, int num2);
int MC_Least(int num, int num2);
int MC_Difference(int num, int num2);
int MC_Abs(int val);
double MC_GetRandomDoubleInRange(double min, double max);
int MC_GetRandomIntInRange(int min, int max);
DoublePoint* MC_ConvertRectToPoints(RECT rect);
BOOL MC_DoLinesIntersect(DoublePoint start, DoublePoint end, DoublePoint start2, DoublePoint end2, POINT* intersection);
BOOL MC_DoesLineIntersectRect(DoublePoint start, DoublePoint end, RECT rect, POINT* intersection);