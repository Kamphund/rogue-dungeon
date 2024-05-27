#pragma once

#include <cmath>

namespace RD
{
	struct Vector2D
	{
		int x = 0;
		int y = 0;

		float Magnitude()
		{
			return std::sqrt(float(x * x + y * y));
		}

		static float Distance(Vector2D a, Vector2D b)
		{
			float xSquared = (a.x - b.x);
			xSquared *= xSquared;
			float ySquared = (a.y - b.y);
			ySquared *= ySquared;
			return std::sqrtf(ySquared + xSquared);
		}
	};

	struct Rect
	{
		int Left;
		int Top;
		int Right;
		int Bottom;

		Vector2D Center()
		{
			Vector2D Vector;
			Vector.y = (Bottom - Top) / 2 + Top;
			Vector.x = (Right - Left) / 2 + Left;
			return Vector;
		}
	};
}
