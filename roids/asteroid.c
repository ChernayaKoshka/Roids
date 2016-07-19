#include "asteroid.h"

extern WindowDetails* details;

extern DLL_Node* root;
extern int nodeCount;

Asteroid* Asteroid_CreateRandomAsteroid()
{
	Asteroid* asteroid = calloc(1, sizeof(Asteroid));

	asteroid->asteroid.right = ASTEROID_WIDTH * 2;
	asteroid->asteroid.bottom = ASTEROID_HEIGHT * 2;

	asteroid->origin.x = MC_GetRandomDoubleInRange(ASTEROID_WIDTH, WINDOW_WIDTH - 20);
	asteroid->origin.y = MC_GetRandomDoubleInRange(ASTEROID_HEIGHT, WINDOW_HEIGHT - 20);

	asteroid->velocity.i = MC_GetRandomDoubleInRange(ASTEROID_MIN_START_VELOCITY, ASTEROID_MAX_START_VELOCITY);
	asteroid->velocity.j = MC_GetRandomDoubleInRange(ASTEROID_MIN_START_VELOCITY, ASTEROID_MAX_START_VELOCITY);
	return asteroid;
}

BOOL Asteroid_CreateNewAsteroid(AsteroidType type, POINT pos)
{
	Asteroid* tail = DLL_GetNodeAt(nodeCount - 1)->data;
	if (tail == NULL) return FALSE;

	Asteroid* newAsteroid = Asteroid_CreateRandomAsteroid();
	if (newAsteroid == NULL) return FALSE;

	newAsteroid->type = type;

	switch (type)
	{
	case LARGE:
		//do nothing, already taken care off in createrandomasteroid
		//this is here in case future developments do something with it
		break;
	case SMALL:
		newAsteroid->asteroid.right /= 2;
		newAsteroid->asteroid.bottom /= 2;
		break;
	}

	if (pos.x >= 0 && pos.y >= 0)
	{
		newAsteroid->origin.x = pos.x;
		newAsteroid->origin.y = pos.y;
	}

	DLL_Node* newAsteroidNode = calloc(1, sizeof(DLL_Node));
	if (newAsteroidNode == NULL) return FALSE;
	newAsteroidNode->data = newAsteroid;

	DLL_AddNode(newAsteroidNode);
	return TRUE;
}

void Asteroid_Destroy(int place)
{
	Asteroid* roid = DLL_GetNodeAt(place)->data;
	if (roid->type == LARGE)
	{
		for (int i = 0; i < 4; i++)
		{
			POINT intPos = { (int)roid->origin.x, (int)roid->origin.y };
			Asteroid_CreateNewAsteroid(SMALL, intPos);
		}
	}

	DLL_RemoveNodeAt(place);
}

BOOL Asteroid_Init()
{
	Asteroid* parentAsteroid = Asteroid_CreateRandomAsteroid(0);
	if (parentAsteroid == NULL) return FALSE;
	root->data = parentAsteroid;

	for (int i = 1; i < 5; i++)
	{
		Asteroid* asteroid = Asteroid_CreateRandomAsteroid(i);
		if (asteroid == NULL) return FALSE;

		DLL_Node* asteroidNode = calloc(1, sizeof(DLL_Node));
		if (asteroidNode == NULL) return FALSE;

		asteroidNode->data = asteroid;
		DLL_AddNode(asteroidNode);
	}

	return TRUE;
}

RECT Asteroid_AdjustRectForOrigin(Asteroid asteroid)
{
	RECT rect = { 0 };

	int right = asteroid.asteroid.right / 2 + (int)asteroid.origin.x;
	int left = -asteroid.asteroid.right / 2 + (int)asteroid.origin.x;

	int top = asteroid.asteroid.bottom / 2 + (int)asteroid.origin.y;
	int bottom = -asteroid.asteroid.bottom / 2 + (int)asteroid.origin.y;

	rect.right = right;
	rect.left = left;

	rect.top = bottom;
	rect.bottom = top;

	return rect;
}

void Asteroid_Update()
{
	static int hitCount = 0; //kind of like a local global
	int rate = (int)ASTEROID_SPAWNRATE; //for some reason I need to define it as a local variable before use... I don't quite understand
										//why, since in the debug window it's the exact same value as if it was just placed directly in the 'if' statement
	if (hitCount == rate)
	{
		hitCount = 0;
		POINT spawnPoint = { 0 };
		int rightSide = rand() % 2;

		if (rightSide)
			spawnPoint.x = WINDOW_WIDTH - ASTEROID_WIDTH - 1; //don't spawn outside of bounds.
		else
			spawnPoint.x = ASTEROID_WIDTH + 1;

		spawnPoint.y = MC_GetRandomIntInRange(ASTEROID_HEIGHT + 1, WINDOW_HEIGHT - ASTEROID_HEIGHT - 1);

		Asteroid_CreateNewAsteroid(LARGE, spawnPoint);
	}
	hitCount++;

	if (nodeCount == 0)
		running = FALSE;

	for (int i = 0; i < nodeCount; i++)
	{
		Asteroid* roid = DLL_GetNodeAt(i)->data;
		if (roid == NULL)
		{
			MessageBox(NULL, L"A null asteroid was found!", L"Oops!", MB_OK);
			running = FALSE;
			return;
		}
		RECT adjustedRect = Asteroid_AdjustRectForOrigin(*roid);

		if (adjustedRect.right + roid->velocity.i > WINDOW_WIDTH - 1 || adjustedRect.left + roid->velocity.i < 0)
		{
			Vector_Invert(&roid->velocity);
			roid->velocity.i += MC_GetRandomDoubleInRange(-0.5, 0.5);
			roid->velocity.j += MC_GetRandomDoubleInRange(-0.5, 0.5);
			return;
		}

		if (adjustedRect.bottom + roid->velocity.j > WINDOW_HEIGHT - 1 || adjustedRect.top + roid->velocity.j < 0)
		{
			Vector_Invert(&roid->velocity);
			roid->velocity.i += MC_GetRandomDoubleInRange(-0.5, 0.5);
			roid->velocity.j += MC_GetRandomDoubleInRange(-0.5, 0.5);
			return;
		}

		roid->origin.x += roid->velocity.i;
		roid->origin.y += roid->velocity.j;
	}
}

void Asteroid_WriteToBuffer()
{
	for (int i = 0; i < nodeCount; i++)
	{
		Asteroid* roid = DLL_GetNodeAt(i)->data;
		if (roid == NULL)
		{
			MessageBox(NULL, L"A null asteroid was found!", L"Oops!", MB_OK);
			running = FALSE;
			return;
		}
		RECT posRect = Asteroid_AdjustRectForOrigin(*roid);

		/*
		1--------2
		|		 |
		|		 |
		4--------3

		*/

		DrawLine(posRect.left, posRect.top, posRect.right, posRect.top, 0x00FFFFFF, details->BackBuffer, details->Width, details->Height); //1->2

		DrawLine(posRect.right, posRect.top, posRect.right, posRect.bottom, 0x00FFFFFF, details->BackBuffer, details->Width, details->Height); //2->3

		DrawLine(posRect.left, posRect.bottom, posRect.right, posRect.bottom, 0x00FFFFFF, details->BackBuffer, details->Width, details->Height); //3->4

		DrawLine(posRect.left, posRect.top, posRect.left, posRect.bottom, 0x00FFFFFF, details->BackBuffer, details->Width, details->Height); //1->4
	}
}