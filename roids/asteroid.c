#include "asteroid.h"

extern WindowDetails* details;

Asteroid roids[5] = { 0 };

BOOL Asteroid_Init()
{
	for (int i = 0; i < 5; i++)
	{
		roids[i].id = i;

		roids[i].asteroid.right = ASTEROID_WIDTH * 2;
		roids[i].asteroid.bottom = ASTEROID_HEIGHT * 2;

		roids[i].origin.x = getRandomDoubleInRange(ASTEROID_WIDTH, WINDOW_WIDTH - 20);
		roids[i].origin.y = getRandomDoubleInRange(ASTEROID_HEIGHT, WINDOW_HEIGHT - 20);

		roids[i].velocity.i = getRandomDoubleInRange(ASTEROID_MIN_START_VELOCITY, ASTEROID_MAX_START_VELOCITY);
		roids[i].velocity.j = getRandomDoubleInRange(ASTEROID_MIN_START_VELOCITY, ASTEROID_MAX_START_VELOCITY);
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
	for (int i = 0; i < 5; i++)
	{
		RECT adjustedRect = Asteroid_AdjustRectForOrigin(roids[i]);

		if (adjustedRect.right + roids[i].velocity.i > WINDOW_WIDTH - 1 || adjustedRect.left + roids[i].velocity.i < 0)
		{
			Vector_Invert(&roids[i].velocity);
			roids[i].velocity.i += getRandomDoubleInRange(-0.5, 0.5);
			roids[i].velocity.j += getRandomDoubleInRange(-0.5, 0.5);
			return;
		}

		if (adjustedRect.bottom + roids[i].velocity.j > WINDOW_HEIGHT - 1 || adjustedRect.top + roids[i].velocity.j < 0)
		{
			Vector_Invert(&roids[i].velocity);
			roids[i].velocity.i += getRandomDoubleInRange(-0.5, 0.5);
			roids[i].velocity.j += getRandomDoubleInRange(-0.5, 0.5);
			return;
		}

		roids[i].origin.x += roids[i].velocity.i;
		roids[i].origin.y += roids[i].velocity.j;
	}
}

void Asteroid_WriteToBuffer()
{
	for (int i = 0; i < 5; i++)
	{
		RECT posRect = Asteroid_AdjustRectForOrigin(roids[i]);

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