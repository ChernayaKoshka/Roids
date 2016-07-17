#include "ship.h"
#include "screen.h"

extern WindowDetails* details;

extern Asteroid roids[5];

Spaceship ship = { 0.0 };

POINT intersection = { 0 };

BOOL Ship_Init()
{
	ship.vector[0].i = 0;
	ship.vector[0].j = DEFAULT_J*SCALAR;

	ship.vector[1].i = -DEFAULT_I;
	ship.vector[1].j = 10 - DEFAULT_J;

	ship.vector[2].i = DEFAULT_I;
	ship.vector[2].j = 10 - DEFAULT_J;

	ship.origin.x = WINDOW_WIDTH / 2;
	ship.origin.y = WINDOW_HEIGHT / 2;

	return TRUE;
}

void Ship_Die()
{
	Ship_ResetVelocity();
	ship.origin.x = WINDOW_WIDTH / 2;
	ship.origin.y = WINDOW_HEIGHT / 2;
}

void Ship_ResetVelocity()
{
	ship.velocity.i = 0.0;
	ship.velocity.j = 0.0;
}

BOOL Ship_Rotate(eRotationDirection direction)
{
	Vector tempVectors[3] = { ship.vector[0], ship.vector[1],ship.vector[2] };
	if (direction == LEFT)
		for (int i = 0; i < 3; i++)
			Vector_Rotate(&(tempVectors[i]), -(M_PI / 20));
	else if (direction == RIGHT)
		for (int i = 0; i < 3; i++)
			Vector_Rotate(&(tempVectors[i]), (M_PI / 20));

	for (int i = 0; i < 3; i++)
	{
		if (ship.origin.x + tempVectors[i].i < 0 || ship.origin.x + tempVectors[i].i > WINDOW_WIDTH)
			return FALSE;
		if (ship.origin.y + tempVectors[i].j < 0 || ship.origin.y + tempVectors[i].j > WINDOW_HEIGHT)
			return FALSE;
	}

	for (int i = 0; i < 3; i++)
	{
		ship.vector[i].i = tempVectors[i].i;
		ship.vector[i].j = tempVectors[i].j;
	}
	return TRUE;
}

BOOL Ship_Accelerate(eMovementDirectionn direction)
{
	if (direction == FORWARD)
	{
		ship.velocity.i = ship.vector[0].i / 20;
		ship.velocity.j = ship.vector[0].j / 20;
	}
	else if (direction == BACKWARD)
	{
		ship.velocity.i = -ship.vector[0].i / 20;
		ship.velocity.j = -ship.vector[0].j / 20;
	}
	else if (direction == STOP)
	{
		Ship_ResetVelocity();
	}

	return TRUE;
}

void Ship_CheckCollisions()
{
	for (int i = 0; i < 3; i++)
	{
		DoublePoint point = { ship.origin.x + ship.vector[i].i, ship.origin.y + ship.vector[i].j };
		for (int j = 0; j < 5; j++)
		{
			RECT adjustedRect = Asteroid_AdjustRectForOrigin(roids[j]);

			DoublePoint* points = calloc(4, sizeof(DoublePoint));

			points[0].x = adjustedRect.left;
			points[0].y = adjustedRect.top;

			points[1].x = adjustedRect.right;
			points[1].y = adjustedRect.top;

			points[2].x = adjustedRect.right;
			points[2].y = adjustedRect.bottom;

			points[3].x = adjustedRect.left;
			points[3].y = adjustedRect.bottom;

			if (Vector_RectContainsPoint(points, point) || Vector_RectContainsPoint(points, ship.origin))
			{
				Ship_Die();
				free(points);
				return;
			}
			free(points);
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
			ship.velocity.i /= 2;
			ship.velocity.j /= 2;
			return;
		}
	}

	ship.origin.x += ship.velocity.i;
	ship.origin.y += ship.velocity.j;
}

void Ship_WriteToBuffer()
{
	for (int i = 0; i < 3; i++)
		DrawLine(ship.origin.x, ship.origin.y, ship.origin.x + ship.vector[i].i, ship.origin.y + ship.vector[i].j, 0x00FF00FF / (i + 1), details->BackBuffer, details->Width, details->Height);

	Vector v1 = { ship.origin.x + ship.vector[0].i, ship.origin.y + ship.vector[0].j };
	Vector v2 = { ship.origin.x + ship.vector[1].i, ship.origin.y + ship.vector[1].j };
	Vector v3 = { ship.origin.x + ship.vector[2].i, ship.origin.y + ship.vector[2].j };

	DrawLine(v1.i, v1.j, v2.i, v2.j, 0x00FF0000, details->BackBuffer, details->Width, details->Height);
	DrawLine(v2.i, v2.j, v3.i, v3.j, 0x0000FF00, details->BackBuffer, details->Width, details->Height);
	DrawLine(v3.i, v3.j, v1.i, v1.j, 0x000000FF, details->BackBuffer, details->Width, details->Height);
}

void Ship_Fire()
{
	ship.shooting = TRUE;

	ship.shooting = FALSE;

	DoublePoint start = { ship.origin.x + ship.vector[0].i , ship.origin.y + ship.vector[0].j };
	DoublePoint end = { ship.origin.x + ship.vector[0].i * SHOT_SCALAR, ship.origin.y + ship.vector[0].j * SHOT_SCALAR };

	for (int i = 0; i < 5; i++)
	{
		RECT adjustedRect = Asteroid_AdjustRectForOrigin(roids[i]);
		if (doesLineIntersectRect(start, end, adjustedRect, &intersection))
		{
			roids[i].velocity.i = 0.0;
			roids[i].velocity.j = 0.0;

			DrawLine(
				ship.origin.x + ship.vector[0].i,
				ship.origin.y + ship.vector[0].j,
				intersection.x,
				intersection.y,
				0x00FFFFFF, details->BackBuffer, details->Width, details->Height);
			return;
		}
	}
	DrawLine(
		ship.origin.x + ship.vector[0].i,
		ship.origin.y + ship.vector[0].j,
		ship.origin.x + ship.vector[0].i * 4,
		ship.origin.y + ship.vector[0].j * 4,
		0x00FFFFFF, details->BackBuffer, details->Width, details->Height);
}