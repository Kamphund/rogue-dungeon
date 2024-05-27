#include "MapGenerator.h"
#include <iostream>
#include <functional>
#include <random>
#include <chrono>

MapGenerator::MapGenerator()
{
	for (int i = 0; i < Settings::MAP_Y; i++)
	{
		for (int j = 0; j < Settings::MAP_X; j++)
		{
			Map[i][j] = 0;
		}
	}

	RD::Rect newRect;
	newRect.Left = 0;
	newRect.Top = 0;
	newRect.Bottom = Settings::MAP_Y;
	newRect.Right = Settings::MAP_X;
	Rooms.push_back(newRect);
}

// These two are just helper functions, would probably use a better library.
bool RandomBool() 
{
	auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	return gen();
}
int RandomIntBetween(int a, int b)
{
	auto gen = std::bind(std::uniform_int_distribution<>(a, b), std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	return gen();
}

void MapGenerator::SplitRoomsBSP()
{
	for (int i = 0; i < SplitAttempts; i++)
	{
		int Random = RandomIntBetween(0, Rooms.size() - 1);
		RD::Rect& Room1 = Rooms[Random];
		RD::Rect Room2 = Room1;

		bool ShouldSplitVertically = RandomBool();
		if (ShouldSplitVertically)
		{
			if ((Room1.Right - Room1.Left) < SplitPadding * 2)
				continue;

			int Split = RandomIntBetween((Room1.Left + SplitPadding), (Room1.Right - SplitPadding));
			Room1.Left = Split;
			Room2.Right = Split;
		}
		else
		{
			if ((Room1.Bottom - Room1.Top) < SplitPadding * 2)
				continue;

			int Split = RandomIntBetween((Room1.Top + SplitPadding), (Room1.Bottom - SplitPadding));
			Room1.Top = Split;
			Room2.Bottom = Split;
		}

		Rooms.push_back(Room2);
	}

	RemoveRandomRooms();
}

void MapGenerator::RemoveRandomRooms()
{
	for (int i = 0; i < RoomsToRemove; i++)
	{
		Rooms.erase(Rooms.begin() + RandomIntBetween(0, (Rooms.size() - 1)));
	}
}

void MapGenerator::GenerateCorridorTiles()
{
	std::vector<RD::Vector2D> RoomCenters;
	for (RD::Rect Room : Rooms)
	{
		RoomCenters.push_back(Room.Center());
	}

	while (RoomCenters.size() > 0)
	{
		RD::Vector2D CurrentRoomCenter = RoomCenters.back();
		RD::Vector2D ClosestRoomCenter;
		float Distance = std::numeric_limits<float>::max();

		for (RD::Vector2D Point : RoomCenters)
		{
			if (Point.x == CurrentRoomCenter.x && Point.y == CurrentRoomCenter.y)
				continue;

			float CompareDistance = RD::Vector2D::Distance(Point, CurrentRoomCenter);
			if (CompareDistance < Distance)
			{
				Distance = CompareDistance;
				ClosestRoomCenter = Point;
			}
		}
		
		if (ClosestRoomCenter.x != 0 && ClosestRoomCenter.y != 0)
		{
			CreateCorridorBetween(CurrentRoomCenter, ClosestRoomCenter);
		}
		RoomCenters.pop_back();
	}
}

void MapGenerator::CreateCorridorBetween(const RD::Vector2D& a, const RD::Vector2D& b)
{
	RD::Vector2D Position = a;
	CorridorTiles.push_back(Position);
	while (Position.y != b.y)
	{
		if (b.y > Position.y)
		{
			Position.y += 1;
		}
		else if (Position.y > b.y)
		{
			Position.y -= 1;
		}
		CorridorTiles.push_back(Position);
	}
	while (Position.x != b.x)
	{
		if (b.x > Position.x)
		{
			Position.x += 1;
		}
		else if (Position.x > b.x)
		{
			Position.x -= 1;
		}
		CorridorTiles.push_back(Position);
	}
}
