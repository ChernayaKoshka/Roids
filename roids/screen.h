#pragma once
#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include "math_custom.h"
#include "ship.h"
#include "asteroid.h"
#include "vector.h"

typedef struct tagWindowDetails
{
	int Width;
	int Height;
	void* BackBuffer;
	HDC DC;
	HWND Window;
	BITMAPINFO BitMapInfo;
}WindowDetails;

extern WindowDetails* details;

extern Spaceship ship;

void Screen_HandleWindowEvents();

BOOL Screen_Init(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd);

void Screen_Render();
