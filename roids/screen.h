#pragma once
#include <Windows.h>
#include "math_custom.h"
#include "vector.h"
#include <assert.h>

#include "math_custom.h"
#include "ship.h"

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

#define CENTER_X ((ship.vector[0].i + ship.vector[1].i + ship.vector[2].i)/3)
#define CENTER_Y ((ship.vector[0].j + ship.vector[1].j + ship.vector[2].j)/3)

void Screen_HandleWindowEvents();

BOOL Screen_Init(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd);

void Screen_Render();
