#include "screen.h"

WindowDetails* details;

Spaceship ship = { 0.0 };

void Screen_HandleWindowEvents()
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam)
{
	POINT newPos = { 0 };

	LRESULT Result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		abort();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			for (int i = 0; i < 3; i++)
				Vector_Rotate(&(ship.vector[i]), -(M_PI / 20));
			break;
		case VK_RIGHT:
			for (int i = 0; i < 3; i++)
				Vector_Rotate(&(ship.vector[i]), (M_PI / 20));
			break;
		case 'W':
			ship.velocity.i = ship.vector[0].i / 20;
			ship.velocity.j = ship.vector[0].j / 20;
			break;
		case 'A':
			break;
		case 'S':
			ship.velocity.i = -ship.vector[0].i / 20;
			ship.velocity.j = -ship.vector[0].j / 20;
			break;
		case 'D':
			break;
		case 'Q':
			ship.velocity.i = 0.0;
			ship.velocity.j = 0.0;
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return Result;
}

WindowDetails* Screen_DefineWindow(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd)
{
	WindowDetails* details = calloc(1, sizeof(WindowDetails));
	if (!details) return NULL;

	details->Height = height;
	details->Width = width;

	int bufferSize = height*width * sizeof(int);
	details->BackBuffer = calloc(1, bufferSize); //4 = bytes to display RGB

	details->BitMapInfo.bmiHeader.biSize = sizeof(details->BitMapInfo.bmiHeader);
	details->BitMapInfo.bmiHeader.biWidth = details->Width;
	details->BitMapInfo.bmiHeader.biHeight = -details->Height;
	details->BitMapInfo.bmiHeader.biPlanes = 1;
	details->BitMapInfo.bmiHeader.biBitCount = 32;
	details->BitMapInfo.bmiHeader.biCompression = BI_RGB;

	//define window
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
#pragma warning(disable : 4028)
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.lpszClassName = className;

	if (!RegisterClassEx(&wc))
		return NULL;

	RECT adjustedRect = { 0 };
	adjustedRect.bottom = height;
	adjustedRect.right = width;
	AdjustWindowRect(&adjustedRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE, FALSE);

	HWND hwndWindow = CreateWindowExW(
		0,
		className,
		windowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Difference(adjustedRect.left, adjustedRect.right),
		Difference(adjustedRect.bottom, adjustedRect.top),
		NULL,
		NULL,
		0,
		0);

	if (!hwndWindow) return NULL;

	details->Window = hwndWindow;
	details->DC = GetDC(hwndWindow);
	ShowWindow(hwndWindow, nShowCmd);

	return details;
}

BOOL Screen_Init(HINSTANCE hInstance, int width, int height, wchar_t* className, wchar_t* windowName, int nShowCmd)
{
	details = Screen_DefineWindow(hInstance, width, height, className, windowName, nShowCmd);
	if (!details) return FALSE;
	return TRUE;
}

void Screen_Render()
{
	StretchDIBits(details->DC,
		0, 0, details->Width, details->Height,
		0, 0, details->BitMapInfo.bmiHeader.biWidth, Abs(details->BitMapInfo.bmiHeader.biHeight),
		details->BackBuffer, &details->BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY);
	ZeroMemory(details->BackBuffer, details->Width*details->Height * sizeof(int));
}