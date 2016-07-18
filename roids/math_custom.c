#include "math_custom.h"
#include <stdio.h>

int Greatest(int num, int num2)
{
	if (num > num2) return num;
	return num2;
}

int Least(int num, int num2)
{
	if (num < num2) return num;
	return num2;
}

int Difference(int num, int num2)
{
	return Greatest(num, num2) - Least(num, num2);
}

int Abs(int val)
{
	if (val < 0) return -val;
	return val;
}

/*
Get Random Float In Range

name: getRandomFloatInRange

desc: This function will generate a random floating point number between
the min and max

params: (1) int min - The minimum size of the floating point number
(2) int max - The maximum size of the floating point number

returns: An integer representing the number of tries allowed to the user
*/
double getRandomDoubleInRange(double min, double max)
{
	return (((double)rand() / RAND_MAX)*(max - min)) + min;
}

int GetColor(double* angle)
{
	*angle += 0.05;
	if (*angle > 2 * M_PI)
		*angle = -(2 * M_PI);
	return RGB(sin(*angle + 0) * 127 + 128, sin(*angle + 2) * 127 + 128, sin(*angle + 4) * 127 + 128);
}

int GetRandRGB()
{
	int color = 0;
	while (!color)
		color = RGB(rand() % 256, rand() % 256, rand() % 256);
	return color;
}

//"special plot" = square plot
void specialPlot(int* screen, int screenWidth, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int z = 0; z < size; z++)
		{
			screen[((y + z)*screenWidth) + x + i] = color;
		}
	}
}

DoublePoint* convertRectToPoints(RECT rect)
{
	DoublePoint* points = malloc(4 * sizeof(DoublePoint));

	int height = Difference(rect.top, rect.bottom);
	int width = Difference(rect.left, rect.right);

	points[2].x = rect.left;
	points[2].y = height;

	points[3].x = width;
	points[3].y = height;

	points[0].x = rect.left;
	points[0].y = Least(rect.top, rect.bottom);

	points[1].x = width;
	points[1].y = Least(rect.top, rect.bottom);

	return points;
}

Slope getSlope(POINT p1, POINT p2)
{
	Slope slope = { 0 };

	if (p2.x - p1.x > 0)
		slope.dx = 1;
	else if (p1.x - p2.x > 0)
		slope.dx = -1;
	else if (p2.x - p1.x == 0)
		slope.dx = 0;

	if (p2.y - p1.y > 0)
		slope.dy = 1;
	else if (p1.y - p2.y > 0)
		slope.dy = -1;
	else if (p2.y - p1.y == 0)
		slope.dy = 0;

	return slope;
}

double getDistance(POINT p1, POINT p2)
{
	return sqrt(((p2.x - p1.x)*(p2.x - p1.x)) + ((p2.y - p1.y)*(p2.y - p1.y)));
}

//most credit -> http://stackoverflow.com/a/3407254/2396111
int roundTo(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (remainder < multiple / 2 + 5)
	{
		return numToRound - remainder;
	}

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}

//all credit -> http://stackoverflow.com/a/3407254/2396111
int roundUpTo(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}

//most credit -> http://stackoverflow.com/a/3407254/2396111
int roundDownTo(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) + remainder);
	else
		return numToRound - remainder;
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the DoublePoint* intersection.
//99.99% credit to http://stackoverflow.com/a/1968345/2396111
BOOL doLinesIntersect(DoublePoint start, DoublePoint end, DoublePoint start2, DoublePoint end2, POINT* intersection)
{
	double s1_x = end.x - start.x;
	double s1_y = end.y - start.y;

	double s2_x = end2.x - start2.x;
	double s2_y = end2.y - start2.y;

	double s = (-s1_y * (start.x - start2.x) + s1_x * (start.y - start2.y)) / (-s2_x * s1_y + s1_x * s2_y);
	double t = (s2_x * (start.y - start2.y) - s2_y * (start.x - start2.x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if (intersection != NULL)
		{
			intersection->x = start.x + (t * s1_x);
			intersection->y = start.y + (t * s1_y);
		}
		return TRUE;
	}

	return FALSE; // No collision
}

void swapPoints(DoublePoint* p1, DoublePoint* p2)
{
	DoublePoint temp = { p1->x, p1->y };
	p1->x = p2->x;
	p1->y = p2->y;

	p2->x = temp.x;
	p2->y = temp.y;
}

void flipRectangle(RECT* rect, BOOL vertical)
{
	RECT temp = { 0 };
	temp.bottom = rect->bottom;
	temp.left = rect->left;

	if (vertical)
	{
		rect->left = rect->right;
		rect->right = temp.left;
	}
	else
	{
		rect->bottom = rect->top;
		rect->top = temp.bottom;
	}
}

void swapLines(DoublePoint* l1start, DoublePoint* l1end, DoublePoint* l2start, DoublePoint* l2end)
{
	swapPoints(l1start, l2start);
	swapPoints(l1end, l2end);
}

BOOL doesLineIntersectRect(DoublePoint start, DoublePoint end, RECT rect, POINT* intersection)
{
	/*
	1-----2
	|	  |
	|	  |
	4-----3
	*/

	DoublePoint l1start = { rect.left, rect.bottom }; //1->2
	DoublePoint l1end = { rect.right, rect.bottom };

	DoublePoint l2start = { rect.right, rect.bottom }; //2->3
	DoublePoint l2end = { rect.right, rect.top };

	DoublePoint l3start = { rect.right, rect.top };//3->4
	DoublePoint l3end = { rect.left, rect.top };

	DoublePoint l4start = { rect.left, rect.top };//4->1
	DoublePoint l4end = { rect.left, rect.bottom };

	if (start.x <= rect.left)
		swapLines(&l2start, &l2end, &l4end, &l4start);

	if (start.y <= rect.top)
		swapLines(&l1start, &l1end, &l3end, &l3start);

	if (doLinesIntersect(start, end, l1start, l1end, intersection)) //top OR bottom
		return TRUE;
	if (doLinesIntersect(start, end, l2start, l2end, intersection)) //right OR left
		return TRUE;

	return FALSE;
}

BOOL doRectanglesOverlap(RECT r1, RECT r2)
{
	if (r1.left < r2.right)
		if (r1.right > r2.left)
			if (r1.top < r2.bottom)
				if (r1.bottom > r2.top)
					return TRUE;
	return FALSE;
}