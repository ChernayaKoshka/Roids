#include "math_custom.h"
#include "drawing.h"

void Plot(int x, int y, int color, int* buffer, int bufferWidth, int bufferHeight)
{
	if (x > bufferWidth) return;
	if (x < 0) return;

	if (y > bufferHeight) return;
	if (y < 0) return;

	if ((y*bufferWidth + x) > (bufferWidth*bufferHeight)) return;

	buffer[y*bufferWidth + x] = color;
}

//code credit of http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
void DrawLine(int x, int y, int x2, int y2, int color, int* buffer, int bufferWidth, int bufferHeight)
{
	//width
	int w = x2 - x;
	//height
	int h = y2 - y;
	//initialize "deltax/y", delta means change
	//so dx1 is "change in x1"
	int dx1 = 0;
	int dy1 = 0;
	int dx2 = 0;
	int dy2 = 0;
	//is our width negative or postive?
	//set deltax1 accordingly
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	//ditto
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	//same as before...
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
	//get |width|
	int longest = MC_Abs(w);
	//get |shortest|
	int shortest = MC_Abs(h);
	//another octant check, is our width less than (or equal to) our height?
	if (!(longest > shortest))
	{
		//swap width/height
		longest = MC_Abs(h);
		shortest = MC_Abs(w);
		//another octant check! Is our height negative? Set deltay accordingly
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		//no change in x2
		dx2 = 0;
	}
	int numerator = longest >> 1; //faster way to divide by two
								  //go along the length of the triangle and...
	for (int i = 0; i <= longest; i++)
	{
		//plot the point at x/y
		Plot(x, y, color, buffer, bufferWidth, bufferHeight);
		//increase the size of the numerator by our "shortest" (h/w, whichever is smaller)
		numerator += shortest;

		//if the numerator is greater than or equal to our "longest" (h/w, whichever is bigger)
		if (!(numerator < longest))
		{
			//decrease the size of the numerator by our "longest"
			numerator -= longest;
			//increase x by our CHANGE IN X
			x += dx1;
			//ditto but with y
			y += dy1;
		}
		else
		{
			//increase our x by our CHANGE IN X2
			x += dx2;
			//ditto but with y
			y += dy2;
		}
	}
}