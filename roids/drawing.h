#pragma once

#include <Windows.h>

#define COLOR_RED 0x00FF0000
#define COLOR_GREEN 0x0000FF00
#define COLOR_BLUE 0x000000FF
#define COLOR_WHITE 0x00FFFFFF
#define COLOR_MAGENTA 0x00FF00FF

typedef struct tagTriangle
{
	POINT left;
	POINT top;
	POINT right;
}TRIANGLE;

void Plot(int x, int y, int color, int* buffer, int bufferWidth, int bufferHeight);

void DrawLine(int x, int y, int x2, int y2, int color, int* buffer, int bufferWidth, int bufferHeight);