#include "RogueDungeon.h"
#include "MapGenerator.h"
#include "Settings.h"
#include "Structures.h"
#include <iostream>	

void RogueDungeon::Run()
{
	MapGenerator Generator;

	Generator.SplitRoomsBSP();

	// Create a char arrays that will be used to print the map.
	char CharMap[Settings::MAP_Y][Settings::MAP_X];

	// Fill map with spaces.
	for (int i = 0; i < Settings::MAP_Y; i++)
	{
		for (int j = 0; j < Settings::MAP_X; j++)
		{
			CharMap[i][j] = ' ';
		}
	}

	// Fill in floor.
	for (int i = 0; i < Generator.Rooms.size(); i++)
	{
		RD::Rect rect = Generator.Rooms[i];
		for (int j = rect.Top + 2; j < rect.Bottom - 2; j++)
		{
			for (int k = rect.Left + 2; k < rect.Right - 2; k++)
			{
				CharMap[j][k] = '.';
			}
		}
	}

	// Add walls to rooms.
	for (int i = 0; i < Generator.Rooms.size(); i++)
	{
		RD::Rect rect = Generator.Rooms[i];

		for (int x = rect.Left + 1; x < rect.Right - 1; x++)
		{
			CharMap[rect.Top + 1][x] = '#';
			CharMap[rect.Bottom - 2][x] = '#';
		}
		for (int y = rect.Top + 1; y < rect.Bottom - 1; y++)
		{
			CharMap[y][rect.Left + 1] = '#';
			CharMap[y][rect.Right - 2] = '#';
		}
	}

	// Add floor on corridor tiles.
	Generator.GenerateCorridorTiles();
	for (int i = 0; i < Generator.CorridorTiles.size(); i++)
	{
		if (Generator.CorridorTiles[i].x == 0 && Generator.CorridorTiles[i].y == 0)
			continue;
		CharMap[Generator.CorridorTiles[i].y][Generator.CorridorTiles[i].x] = '.';
	}

	// Add walls to corridors.
	for (int i = 0; i < Generator.CorridorTiles.size(); i++)
	{
		if (Generator.CorridorTiles[i].x == 0 && Generator.CorridorTiles[i].y == 0)
			continue;
		for (int y = Generator.CorridorTiles[i].y - 1; y < Generator.CorridorTiles[i].y + 2; y++)
		{
			for (int x = Generator.CorridorTiles[i].x - 1; x < Generator.CorridorTiles[i].x + 2; x++)
			{
				if (CharMap[y][x] == ' ')
				{
					CharMap[y][x] = '#';
				}
			}
		}
	}

	// Add doors to rooms.
	// We scan the bounds of the room and replace any "floor" tile to a "door".
	for (RD::Rect Room : Generator.Rooms)
	{
		for (int x = Room.Left + 1; x < Room.Right - 1; x++)
		{
			if (CharMap[Room.Top + 1][x] == '.')
			{
				CharMap[Room.Top + 1][x] = '+';
			}
			if (CharMap[Room.Bottom - 2][x] == '.')
			{
				CharMap[Room.Bottom - 2][x] = '+';
			}
		}
		for (int y = Room.Top + 1; y < Room.Bottom - 1; y++)
		{
			if (CharMap[y][Room.Left + 1] == '.')
			{
				CharMap[y][Room.Left + 1] = '+';
			}
			if (CharMap[y][Room.Right - 2] == '.')
			{
				CharMap[y][Room.Right - 2] = '+';
			}
		}
	}

	// Print the map.
	for (int i = 0; i < Settings::MAP_Y; i++)
	{
		for (int j = 0; j < Settings::MAP_X; j++)
		{
			std::cout << CharMap[i][j];
		}
		std::cout << "\n";
	}
}
