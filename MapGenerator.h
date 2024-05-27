#pragma once

#include "Settings.h"
#include "Structures.h"
#include <vector>

class MapGenerator
{
public:

	MapGenerator();

	int Map[Settings::MAP_Y][Settings::MAP_X];

	std::vector<RD::Rect> Rooms;
	std::vector<RD::Vector2D> CorridorTiles;

	void SplitRoomsBSP();

	void GenerateCorridorTiles();

private:

	int SplitPadding = 6;
	int RoomsToRemove = 12;
	int SplitAttempts = 40;

	void RemoveRandomRooms();

	void CreateCorridorBetween(const RD::Vector2D& a, const RD::Vector2D& b);

};

