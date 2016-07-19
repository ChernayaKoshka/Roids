#include "math_custom.h"
#include <stdio.h>

int MC_Greatest(int num, int num2)
{
	if (num > num2) return num;
	return num2;
}

int MC_Least(int num, int num2)
{
	if (num < num2) return num;
	return num2;
}

int MC_Difference(int num, int num2)
{
	return MC_Greatest(num, num2) - MC_Least(num, num2);
}

int MC_Abs(int val)
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
double MC_GetRandomDoubleInRange(double min, double max)
{
	return (((double)rand() / RAND_MAX)*(max - min)) + min;
}

int MC_GetRandomIntInRange(int min, int max)
{
	return rand() % (max + 1 - min) + min;
}

DoublePoint* MC_ConvertRectToPoints(RECT rect)
{
	DoublePoint* points = calloc(4, sizeof(DoublePoint));

	int height = MC_Difference(rect.top, rect.bottom);
	int width = MC_Difference(rect.left, rect.right);

	points[2].x = rect.left;
	points[2].y = height;

	points[3].x = width;
	points[3].y = height;

	points[0].x = rect.left;
	points[0].y = MC_Least(rect.top, rect.bottom);

	points[1].x = width;
	points[1].y = MC_Least(rect.top, rect.bottom);

	return points;
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the DoublePoint* intersection.
//99.99% credit to http://stackoverflow.com/a/1968345/2396111
BOOL MC_DoLinesIntersect(DoublePoint start, DoublePoint end, DoublePoint start2, DoublePoint end2, POINT* intersection)
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
			intersection->x = (long)(start.x + (t * s1_x));
			intersection->y = (long)(start.y + (t * s1_y));
		}
		return TRUE;
	}

	return FALSE; // No collision
}

void MC_FlipRectangle(RECT* rect, BOOL vertical)
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

void MC_SwapPoints(DoublePoint* p1, DoublePoint* p2)
{
	DoublePoint temp = { p1->x, p1->y };
	p1->x = p2->x;
	p1->y = p2->y;

	p2->x = temp.x;
	p2->y = temp.y;
}

void MC_SwapLines(DoublePoint* l1start, DoublePoint* l1end, DoublePoint* l2start, DoublePoint* l2end)
{
	MC_SwapPoints(l1start, l2start);
	MC_SwapPoints(l1end, l2end);
}

BOOL MC_DoesLineIntersectRect(DoublePoint start, DoublePoint end, RECT rect, POINT* intersection)
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
		MC_SwapLines(&l2start, &l2end, &l4end, &l4start);

	if (start.y <= rect.top)
		MC_SwapLines(&l1start, &l1end, &l3end, &l3start);

	if (MC_DoLinesIntersect(start, end, l1start, l1end, intersection)) //top OR bottom
		return TRUE;
	if (MC_DoLinesIntersect(start, end, l2start, l2end, intersection)) //right OR left
		return TRUE;

	return FALSE;
}