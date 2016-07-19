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
#include "asteroid.h"
#include "doubly_linked_list.h"

#if DEBUG_OUTPUT
wchar_t* dbgBuffer[513];
#endif

extern WindowDetails* details;

extern Spaceship ship;

void update()
{
	Ship_Update();
	Asteroid_Update();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	/*INIT*/
	srand((unsigned int)time(NULL));

	if (!Time_Init())
	{
		MessageBoxW(NULL, L"Get a newer operating system.", L"What is this, the stone age?", MB_OK);
		return -1;
	}

	Ship_Init();

	if (!DLL_Init()) //Doubly Linked List, not Dynamic Link Library :)
		return -1; //no memory, no point in error message

	if (!Asteroid_Init())
		return -1; //no memory, no point in error message

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
			Ship_CheckCollisions();
			timeAccumulated -= STEPS_PER_SECOND;
		}

		wchar_t buf[256];
		swprintf_s(buf, 256, L"Score: %d", ship.score);
		TextOut(details->DC, 0, 0, buf, lstrlen(buf));

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

	if (nodeCount == 0) // no asteroids left
		MessageBox(NULL, L"YOU WON!", L"Congrats", MB_OK);

	return EXIT_SUCCESS;
}