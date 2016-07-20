#include "ship.h"
#include "screen.h"

extern WindowDetails* details;

Spaceship ship = { 0.0 };

void Ship_Init()
{
	ship.vector[NOSE].i = 0;
	ship.vector[NOSE].j = DEFAULT_J*SCALAR;

	ship.vector[LEFT].i = -DEFAULT_I;
	ship.vector[LEFT].j = 10 - DEFAULT_J;

	ship.vector[RIGHT].i = DEFAULT_I;
	ship.vector[RIGHT].j = 10 - DEFAULT_J;

	ship.origin.x = WINDOW_WIDTH / 2;
	ship.origin.y = WINDOW_HEIGHT / 2;

	return;
}

void Ship_Die()
{
	Ship_ResetVelocity();
	ship.origin.x = WINDOW_WIDTH / 2;
	ship.origin.y = WINDOW_HEIGHT / 2;
	ship.score = 0;
}

void Ship_ResetVelocity()
{
	ship.velocity.i = 0.0;
	ship.velocity.j = 0.0;
}

BOOL Ship_Rotate(RotationDirection direction)
{
	Vector tempVectors[3] = { ship.vector[NOSE], ship.vector[LEFT],ship.vector[RIGHT] };
	switch (direction)
	{
	case ROTATE_LEFT:
		for (int i = 0; i < 3; i++)
		{
			Vector_Rotate(&tempVectors[i], -(M_PI / SHIP_ROTATION_LIMITER));
		}
		break;
	case ROTATE_RIGHT:
		for (int i = 0; i < 3; i++)
		{
			Vector_Rotate(&tempVectors[i], (M_PI / SHIP_ROTATION_LIMITER));
		}
		break;
	}

	for (int i = 0; i < 3; i++)
	{
		if (ship.origin.x + tempVectors[i].i < 0 || ship.origin.x + tempVectors[i].i > WINDOW_WIDTH)
		{
			return FALSE;
		}
		if (ship.origin.y + tempVectors[i].j < 0 || ship.origin.y + tempVectors[i].j > WINDOW_HEIGHT)
		{
			return FALSE;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		ship.vector[i].i = tempVectors[i].i;
		ship.vector[i].j = tempVectors[i].j;
	}
	return TRUE;
}

BOOL Ship_Accelerate(MovementDirectionn direction)
{
	switch (direction)
	{
	case FORWARD:
		ship.velocity.i = ship.vector[NOSE].i / SHIP_VELOCITY_LIMITING_SCALAR; //divided just to scale it so it's not freaky fast
		ship.velocity.j = ship.vector[NOSE].j / SHIP_VELOCITY_LIMITING_SCALAR;
		break;
	case BACKWARD:
		ship.velocity.i = -ship.vector[NOSE].i / SHIP_VELOCITY_LIMITING_SCALAR;
		ship.velocity.j = -ship.vector[NOSE].j / SHIP_VELOCITY_LIMITING_SCALAR;
		break;
	case STOP:
		Ship_ResetVelocity();
		break;
	}
	return TRUE;
}

void Ship_CheckCollisions()
{
	for (int i = 0; i < 3; i++)
	{
		DoublePoint point = { ship.origin.x + ship.vector[i].i, ship.origin.y + ship.vector[i].j };
		for (int j = 0; j < nodeCount; j++)
		{
			Asteroid* roid = DLL_GetNodeAt(j)->data;
			if (roid == NULL)
			{
				MessageBox(NULL, L"A null asteroid was found!", L"Oops!", MB_OK);
				running = FALSE;
				return;
			}

			RECT adjustedRect = Asteroid_AdjustRectForOrigin(*roid);

			DoublePoint points[4];

			points[0].x = adjustedRect.left;
			points[0].y = adjustedRect.top;

			points[1].x = adjustedRect.right;
			points[1].y = adjustedRect.top;

			points[2].x = adjustedRect.right;
			points[2].y = adjustedRect.bottom;

			points[3].x = adjustedRect.left;
			points[3].y = adjustedRect.bottom;

			if (Vector_RectContainsPoint(points, point) ||
				Vector_RectContainsPoint(points, ship.origin))
			{
				Ship_Die();
				return;
			}
		}
	}
}

void Ship_Update()
{
	for (int i = 0; i < 3; i++)
	{
		if (ship.origin.x + ship.vector[i].i + ship.velocity.i < 0 ||
			ship.origin.x + ship.vector[i].i + ship.velocity.i > WINDOW_WIDTH - 1 ||
			ship.origin.y + ship.vector[i].j + ship.velocity.j < 0 ||
			ship.origin.y + ship.vector[i].j + ship.velocity.j > WINDOW_HEIGHT - 1)
		{
			Vector_Invert(&ship.velocity);
			ship.velocity.i /= SHIP_BUMP_VELOCCITY_REDUCTION_SCALAR;
			ship.velocity.j /= SHIP_BUMP_VELOCCITY_REDUCTION_SCALAR;
			return;
		}
	}

	ship.origin.x += ship.velocity.i;
	ship.origin.y += ship.velocity.j;
}

void Ship_WriteToBuffer()
{
	for (int i = 0; i < 3; i++)
	{
		DrawLine(
			(int)(ship.origin.x),
			(int)(ship.origin.y),
			(int)(ship.origin.x + ship.vector[i].i),
			(int)(ship.origin.y + ship.vector[i].j),
			COLOR_MAGENTA / (i + 1), //changes color
			details->BackBuffer,
			details->Width,
			details->Height);
	}

	Vector v1 = { ship.origin.x + ship.vector[NOSE].i, ship.origin.y + ship.vector[NOSE].j };
	Vector v2 = { ship.origin.x + ship.vector[LEFT].i, ship.origin.y + ship.vector[LEFT].j };
	Vector v3 = { ship.origin.x + ship.vector[RIGHT].i, ship.origin.y + ship.vector[RIGHT].j };

	DrawLine((int)v1.i, (int)v1.j, (int)v2.i, (int)v2.j, COLOR_RED, details->BackBuffer, details->Width, details->Height);
	DrawLine((int)v2.i, (int)v2.j, (int)v3.i, (int)v3.j, COLOR_GREEN, details->BackBuffer, details->Width, details->Height);
	DrawLine((int)v3.i, (int)v3.j, (int)v1.i, (int)v1.j, COLOR_BLUE, details->BackBuffer, details->Width, details->Height);
}

void Ship_Fire()
{
	DoublePoint start = { ship.origin.x + ship.vector[NOSE].i , ship.origin.y + ship.vector[NOSE].j };
	DoublePoint end = { ship.origin.x + ship.vector[NOSE].i * SHOT_LENGTH_SCALAR, ship.origin.y + ship.vector[NOSE].j * SHOT_LENGTH_SCALAR };

	POINT intersection;
	for (int i = 0; i < nodeCount; i++)
	{
		Asteroid* roid = (Asteroid*)DLL_GetNodeAt(i)->data;
		if (roid == NULL)
		{
			MessageBox(NULL, L"A null asteroid was found!", L"Oops!", MB_OK);
			running = FALSE;
			return;
		}

		RECT adjustedRect = Asteroid_AdjustRectForOrigin(*roid);
		if (MC_DoesLineIntersectRect(start, end, adjustedRect, &intersection))
		{
			switch (roid->type)
			{
			case LARGE:
				ship.score += 5;
				break;
			case SMALL:
				ship.score++;
				break;
			}

			DrawLine(
				(int)(ship.origin.x + ship.vector[NOSE].i),
				(int)(ship.origin.y + ship.vector[NOSE].j),
				intersection.x,
				intersection.y,
				COLOR_WHITE, details->BackBuffer, details->Width, details->Height);
			Asteroid_Destroy(i);
			return;
		}
	}

	DrawLine(
		(int)(ship.origin.x + ship.vector[NOSE].i),
		(int)(ship.origin.y + ship.vector[NOSE].j),
		(int)(ship.origin.x + ship.vector[NOSE].i * SHOT_LENGTH_SCALAR),
		(int)(ship.origin.y + ship.vector[NOSE].j * SHOT_LENGTH_SCALAR),
		COLOR_WHITE, details->BackBuffer, details->Width, details->Height);
}