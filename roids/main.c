#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <ShlObj.h>
#include <process.h>
#include "screen.h"
#include "timer.h"
#include "globals.h"
#include "ship.h"
#include "drawing.h"

#if DEBUG_OUTPUT
wchar_t* dbgBuffer[513];
#endif

extern WindowDetails* details;

extern Spaceship ship;

BOOL running = TRUE;

void update()
{
	Ship_Update();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	/*INIT*/
	if (!Time_Init())
	{
		MessageBoxW(NULL, L"Get a newer operating system.", L"What is this, the stone age?", MB_OK);
		return -1;
	}

	if (!Ship_Init())
	{
		MessageBox(NULL, "Honestly, I have no idea how this could have failed.", "???", MB_OK);
		return -1;
	}

	if (!Screen_Init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CLASS, WINDOW_TITLE, nShowCmd)) //gotta make sure that player/level are initialized before creating window
	{
		MessageBox(NULL, L"Unable to Screen_Init!", L"PANIC!", MB_OK);
		return -1;
	}
	/*END INIT*/

	float prevTime = 0;
	float timeAccumulated = 0;

	while (running)
	{
		Screen_HandleWindowEvents();
		float time = Time_Get();
		timeAccumulated += time - prevTime;
		prevTime = time;

		if (timeAccumulated >= STEPS_PER_SECOND)
		{
			update();
			Screen_Render();
			timeAccumulated -= STEPS_PER_SECOND;
		}

#if DEBUG_OUTPUT
		for (int i = 0; i < 3; i++)
		{
			swprintf_s(dbgBuffer, 512, L"Vec#%d: <%lf,%lf>", i + 1, ship.vector[i].i, ship.vector[i].j);
			TextOutW(details->DC, 0, i * 20, dbgBuffer, lstrlenW(dbgBuffer));
		}

		swprintf_s(dbgBuffer, 512, L"Origin: (%lf, %lf)", ship.origin.x, ship.origin.y);
		TextOutW(details->DC, 0, 3 * 20, dbgBuffer, lstrlenW(dbgBuffer));

		swprintf_s(dbgBuffer, 512, L"Velocity: <%lf, %lf>", ship.velocity.i, ship.velocity.j);
		TextOutW(details->DC, 0, 4 * 20, dbgBuffer, lstrlenW(dbgBuffer));
#endif
	}

	return EXIT_SUCCESS;
}