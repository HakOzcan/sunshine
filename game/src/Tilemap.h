#pragma once
#include "raylib.h"
#include "TileCoord.h"

#define MAP_WIDTH 32
#define MAP_HEIGHT 32

enum class Tile
{
	Floor = 0,
	Wall,
	Count
};

class Tilemap
{
public:
	float tileSizeX = 32;
	float tileSizeY = 32;
	Color tileColors[(int)Tile::Count];

	Tile tiles[MAP_WIDTH][MAP_HEIGHT];

	Vector2 GetScreenPositionOfTile(TileCoord coordinate)
	{
		return { coordinate.x * tileSizeX, coordinate.y * tileSizeY };
	}

	Tilemap()
	{
		tileColors[(int)Tile::Floor] = YELLOW;
		tileColors[(int)Tile::Wall] = RED;
	}

	void RandomizeMap()
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				tiles[x][y] = (Tile) (rand() % (int)Tile::Count);
			}
		}
	}

	void DrawBorders(Color color = BLACK)
	{
		for (int x = 0; MAP_WIDTH; x++)
			DrawLine(x * tileSizeX, 0, x * tileSizeX, MAP_HEIGHT * tileSizeY, color);
		for (int y = 0; MAP_HEIGHT; y++)
			DrawLine(0, y * tileSizeY, MAP_WIDTH * tileSizeX, y * tileSizeY, color);
	}



	void DrawTiles()
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				Tile tileType = tiles[x][y];
				Color tileColor = tileColors[(int)tileType];

				DrawRectangle(x * tileSizeX, y * tileSizeY, tileSizeX, tileSizeY, tileColor);
			}
		}
	}
	
};
