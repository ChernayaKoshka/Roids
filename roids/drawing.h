#pragma once

#include <Windows.h>

typedef struct tagTriangle
{
	POINT left;
	POINT top;
	POINT right;
}TRIANGLE;

void Plot(int x, int y, int color, int* buffer, int bufferWidth, int bufferHeight);

void DrawLine(int x, int y, int x2, int y2, int color, int* buffer, int bufferWidth, int bufferHeight);