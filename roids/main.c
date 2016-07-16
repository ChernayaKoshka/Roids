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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define WINDOW_TITLE L"'ROIDS"
#define WINDOW_CLASS L"roids"

#define DEFAULT_J 20
#define DEFAULT_I 15
#define SCALAR 2

#if DEBUG_OUTPUT
wchar_t* dbgBuffer[513];
#endif

extern WindowDetails* details;

extern Spaceship ship;

BOOL running = TRUE;

void update(float timeAccumulated)
{
	//Enitites_Update();
	//Player_Update();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	/*INIT*/
	if (!Time_Init())
	{
		MessageBoxW(NULL, L"Get a newer operating system.", L"What is this, the stone age?", MB_OK);
		return -1;
	}

	ship.vector[0].i = 0;
	ship.vector[0].j = DEFAULT_J*SCALAR;

	ship.vector[1].i = -DEFAULT_I;
	ship.vector[1].j = 10 - DEFAULT_J;

	ship.vector[2].i = DEFAULT_I;
	ship.vector[2].j = 10 - DEFAULT_J;

	ship.origin.x = WINDOW_WIDTH / 2;
	ship.origin.y = WINDOW_HEIGHT / 2;

	//if (!Entities_Init())
	//{
	//	MessageBox(NULL, L"Unable to Entities_Init!", L"Unable to Entities_Init!", MB_OK);
	//	return -1;
	//}
	//
	//if (!Player_Init())
	//{
	//	MessageBox(NULL, L"Unable to Player_Init!", L"Unable to Player_Init!", MB_OK);
	//	return -1;
	//}

	if (!Screen_Init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CLASS, WINDOW_TITLE, nShowCmd)) //gotta make sure that player/level are initialized before creating window
	{
		MessageBox(NULL, L"Unable to Screen_Init!", L"Unable to Screen_Init!", MB_OK);
		return -1;
	}
	/*END INIT*/

	float prevTime = 0;
	float timeAccumulated = 0;

	HDC dc = GetDC(details->Window);

	while (running)
	{
		Screen_HandleWindowEvents();
		float time = Time_Get();
		timeAccumulated += time - prevTime;
		prevTime = time;

		if (timeAccumulated >= STEPS_PER_SECOND)
		{
			ship.origin.x += ship.velocity.i;
			ship.origin.y += ship.velocity.j;
			//update(timeAccumulated);
			for (int i = 0; i < 3; i++)
				DrawLine(ship.origin.x, ship.origin.y, ship.origin.x + ship.vector[i].i, ship.origin.y + ship.vector[i].j, 0x00FF00FF / (i + 1), details->BackBuffer, details->Width);

			Vector v1 = { ship.origin.x + ship.vector[0].i, ship.origin.y + ship.vector[0].j };
			Vector v2 = { ship.origin.x + ship.vector[1].i, ship.origin.y + ship.vector[1].j };
			Vector v3 = { ship.origin.x + ship.vector[2].i, ship.origin.y + ship.vector[2].j };

			DrawLine(v1.i, v1.j, v2.i, v2.j, 0x00FF0000, details->BackBuffer, details->Width);
			DrawLine(v2.i, v2.j, v3.i, v3.j, 0x0000FF00, details->BackBuffer, details->Width);
			DrawLine(v3.i, v3.j, v1.i, v1.j, 0x000000FF, details->BackBuffer, details->Width);

			Screen_Render();

			timeAccumulated -= STEPS_PER_SECOND;
		}

#if DEBUG_OUTPUT
		for (int i = 0; i < 3; i++)
		{
			swprintf_s(dbgBuffer, 512, L"Vec#%d: <%lf,%lf>", i + 1, ship.vector[i].i, ship.vector[i].j);
			TextOutW(dc, 0, i * 20, dbgBuffer, lstrlenW(dbgBuffer));
		}

		swprintf_s(dbgBuffer, 512, L"Origin: (%lf, %lf)", ship.origin.x, ship.origin.y);
		TextOutW(dc, 0, 3 * 20, dbgBuffer, lstrlenW(dbgBuffer));

		swprintf_s(dbgBuffer, 512, L"Velocity: <%lf, %lf>", ship.velocity.i, ship.velocity.j);
		TextOutW(dc, 0, 4 * 20, dbgBuffer, lstrlenW(dbgBuffer));
#endif
	}

	return EXIT_SUCCESS;
}