#include "screen.h"

WindowDetails* details;

extern BOOL running;

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
	LRESULT Result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		running = FALSE;
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'Z': case VK_NUMPAD1:
			Ship_Fire();
			break;
		case 'A': case VK_LEFT:
			Ship_Rotate(ROTATE_LEFT);
			break;
		case 'D': case VK_RIGHT:
			Ship_Rotate(ROTATE_RIGHT);
			break;
		case 'W': case VK_UP:
			Ship_Accelerate(FORWARD);
			break;
		case 'S': case VK_DOWN:
			Ship_Accelerate(BACKWARD);
			break;
		case 'Q': case VK_NUMPAD0:
			Ship_Accelerate(STOP);
			break;
		}
		break;
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
		MC_Difference(adjustedRect.left, adjustedRect.right),
		MC_Difference(adjustedRect.bottom, adjustedRect.top),
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
	Ship_WriteToBuffer();
	Asteroid_WriteToBuffer();

	StretchDIBits(details->DC,
		0, 0, details->Width, details->Height,
		0, 0, details->BitMapInfo.bmiHeader.biWidth, MC_Abs(details->BitMapInfo.bmiHeader.biHeight),
		details->BackBuffer, &details->BitMapInfo,
		DIB_RGB_COLORS, SRCCOPY);
	ZeroMemory(details->BackBuffer, details->Width*details->Height * sizeof(int));
}