#include "asteroid.h"

extern WindowDetails* details;

extern SLL_Node* root;
extern int nodeCount;

Asteroid* Asteroid_CreateRandomAsteroid(int id)
{
	Asteroid* asteroid = calloc(1, sizeof(Asteroid));
	asteroid->id = id;

	asteroid->asteroid.right = ASTEROID_WIDTH * 2;
	asteroid->asteroid.bottom = ASTEROID_HEIGHT * 2;

	asteroid->origin.x = getRandomDoubleInRange(ASTEROID_WIDTH, WINDOW_WIDTH - 20);
	asteroid->origin.y = getRandomDoubleInRange(ASTEROID_HEIGHT, WINDOW_HEIGHT - 20);
	asteroid->velocity.i = getRandomDoubleInRange(ASTEROID_MIN_START_VELOCITY, ASTEROID_MAX_START_VELOCITY);
	asteroid->velocity.j = getRandomDoubleInRange(ASTEROID_MIN_START_VELOCITY, ASTEROID_MAX_START_VELOCITY);
	return asteroid;
}

BOOL Asteroid_CreateNewAsteroid()
{
	Asteroid* tail = SLL_GetNodeAt(nodeCount - 1)->data;
	if (tail == NULL) return FALSE;

	Asteroid* newAsteroid = Asteroid_CreateRandomAsteroid(++tail->id);
	if (newAsteroid == NULL) return FALSE;

	SLL_Node* newAsteroidNode = calloc(1, sizeof(SLL_Node));
	if (newAsteroidNode == NULL) return FALSE;
	newAsteroidNode->data = newAsteroid;

	SLL_AddNode(newAsteroidNode);
	return TRUE;
}

BOOL Asteroid_Init()
{
	Asteroid* parentAsteroid = Asteroid_CreateRandomAsteroid(0);
	root->data = parentAsteroid;

	for (int i = 1; i < 5; i++)
	{
		Asteroid* asteroid = Asteroid_CreateRandomAsteroid(i);
		SLL_Node* asteroidNode = calloc(1, sizeof(SLL_Node));
		asteroidNode->data = asteroid;
		SLL_AddNode(asteroidNode);
	}

	return TRUE;
}

RECT Asteroid_AdjustRectForOrigin(Asteroid asteroid)
{
	RECT rect = { 0 };

	int right = asteroid.asteroid.right / 2 + asteroid.origin.x;
	int left = -asteroid.asteroid.right / 2 + asteroid.origin.x;

	int top = asteroid.asteroid.bottom / 2 + asteroid.origin.y;
	int bottom = -asteroid.asteroid.bottom / 2 + asteroid.origin.y;

	rect.right = right;
	rect.left = left;

	rect.top = bottom;
	rect.bottom = top;

	return rect;
}

void Asteroid_Update()
{
	for (int i = 0; i < nodeCount; i++)
	{
		Asteroid* roid = SLL_GetNodeAt(i)->data;
		if (roid == NULL)
			continue; //TODO: Error message
		RECT adjustedRect = Asteroid_AdjustRectForOrigin(*roid);

		if (adjustedRect.right + roid->velocity.i > WINDOW_WIDTH - 1 || adjustedRect.left + roid->velocity.i < 0)
		{
			Vector_Invert(&roid->velocity);
			roid->velocity.i += getRandomDoubleInRange(-0.5, 0.5);
			roid->velocity.j += getRandomDoubleInRange(-0.5, 0.5);
			return;
		}

		if (adjustedRect.bottom + roid->velocity.j > WINDOW_HEIGHT - 1 || adjustedRect.top + roid->velocity.j < 0)
		{
			Vector_Invert(&roid->velocity);
			roid->velocity.i += getRandomDoubleInRange(-0.5, 0.5);
			roid->velocity.j += getRandomDoubleInRange(-0.5, 0.5);
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
		Asteroid* roid = SLL_GetNodeAt(i)->data;
		if (roid == NULL)
			continue; //TODO: Error message
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