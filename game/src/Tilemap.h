#pragma once
#include "raylib.h"
#include "TileCoord.h"
#include <string>
#include<vector>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

enum class Tile
{
	Floor = 0,
	Wall,
	Count
};

class Tilemap
{
public:
	float tileSizeX = 100;
	float tileSizeY = 100;
	const Vector2 NORTH = { -1,0 };
	const Vector2 SOUTH = { 1,0 };
	const Vector2 EAST = { 0,1 };
	const Vector2 WEST = { 0,-1 };
	Vector2 ratPos;
	float ratScreenPosX;
	float ratScreenPosY;
	Color tileColors[(int)Tile::Count];

	Tile tiles[MAP_WIDTH][MAP_HEIGHT];

	Tilemap()
	{
		tileColors[(int)Tile::Floor] = WHITE;
		tileColors[(int)Tile::Wall] = BLACK;
	}

	Tilemap(Vector2 ratPosition)
	{
		ratPos = ScreenPosToTilePos(ratPosition);
		ratScreenPosX = TilePosToScreenPos(ratPos).x;
		ratScreenPosY = TilePosToScreenPos(ratPos).y;
	}

	int GetGridWidth() { return MAP_WIDTH; }
	int GetGridHeight() { return MAP_HEIGHT; }

	Tile& GetTile(int x, int y)
	{
		Tile tileType = tiles[x][y];
		return tileType;
	}

	void SetTile(int x, int y, Tile type)
	{
		tiles[x][y] = type;
	}

	bool IsItFloor(int x, int y)
	{
		bool isItFloor = false;
		if (GetTile(static_cast<int>(x), static_cast<int>(y)) == Tile::Floor)
		{
			isItFloor = true;
		}
		return isItFloor;
	}

	bool IsInsideGrid(int x, int y)
	{
		bool isInside = false;
		if (x >= 0 && x < GetGridWidth())
		{
			if (y >= 0 && y < GetGridHeight())
			{
				isInside = true;
			}
		}
		return isInside;
	}

	Vector2 GetScreenPositionOfTile(TileCoord coordinate)
	{
		return Vector2{ static_cast<float>(coordinate.x * tileSizeX),static_cast<float>(coordinate.y * tileSizeY) };
	}

	Vector2 ScreenPosToTilePos(Vector2 positionOnScreen)
	{
		return { floorf(positionOnScreen.x / tileSizeX), floorf(positionOnScreen.y / tileSizeY) };
	}

	Vector2 TilePosToScreenPos(Vector2 positionOnTile)
	{
		return Vector2{ static_cast<float>(positionOnTile.x * tileSizeX),static_cast<float>(positionOnTile.y * tileSizeY) };
	}

	void RandomizeMap()
	{
		for (int x = 0; x < this->GetGridWidth(); x++)
		{
			for (int y = 0; y < this->GetGridHeight(); y++)
			{
				int chanceOfWall = 40;
				int rollForWall = rand() % 100;

				if (rollForWall >= chanceOfWall)
					SetTile(x, y, Tile::Floor);
				else
					SetTile(x, y, Tile::Wall);

			}
		}
	}

	void DrawBorders(Color color = BLACK)
	{
		for (int x = 0; x <= GetGridWidth(); x++)
			DrawLine(x * tileSizeX, 0, x * tileSizeX, GetGridHeight() * tileSizeY, color);
		for (int y = 0; y <= GetGridHeight(); y++)
			DrawLine(0, y * tileSizeY, GetGridWidth() * tileSizeX, y * tileSizeY, color);
	}

	bool IsTraversible(Vector2 tilePosition)
	{
		//static bool isTraversible = false;
		Vector2 tilePos = tilePosition;
		if (IsInsideGrid(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y)))
		{
			//tiles[1][1] = GetTile(static_cast<int>(tilePosition.x), static_cast<int>(tilePosition.y));
			//tiles[(int)tilePos.x][(int)tilePos.y] = GetTile(static_cast<float>(tilePos.x), static_cast<float>(tilePos.y));
			int x = tilePosition.x;
			int y = tilePosition.y;
			if (tiles[x][y] == Tile::Floor) return true;
		}
		return false;
	}

	//bool IsTraversible(Vector2 tilePosition)
	//{
	//	bool isTraversible = false;
	//	if (GetTile(static_cast<int>(tilePosition.x), static_cast<int>(tilePosition.y)) == Tile::Floor)
	//	{
	//		isTraversible = true;
	//	}
	//	return isTraversible;
	//}

	std::vector<Vector2> GetTraversibleTilesAdjacentTo(Vector2 tilePos)
	{
		std::vector<Vector2> adjacentTilePositions;

		Vector2 N = tilePos + NORTH;
		Vector2 S = tilePos + SOUTH;
		Vector2 E = tilePos + EAST;
		Vector2 W = tilePos + WEST;
		if (IsTraversible(N)) adjacentTilePositions.push_back(N);
		if (IsTraversible(S)) adjacentTilePositions.push_back(S);
		if (IsTraversible(E)) adjacentTilePositions.push_back(E);
		if (IsTraversible(W)) adjacentTilePositions.push_back(W);

		return adjacentTilePositions;
	}

	void DrawTiles()
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				Tile tileType = tiles[x][y];
				TileCoord coordinatesOfTile = { x,y };
				Vector2 vec2CoordinatesOfTile = { x,y };
				float addForCenterX = tileSizeX / 2;
				float addForCenterY = tileSizeY / 2;
				Color tileColor = tileColors[static_cast<int>(tileType)];
				std::string tileCoordinatesToDraw = std::to_string(x) + "," + std::to_string(y);

				DrawRectangle(x * tileSizeX, y * tileSizeY, tileSizeX, tileSizeY, tileColor);
				DrawText(tileCoordinatesToDraw.c_str(), x * tileSizeX + 5, y * tileSizeY + 5, 10, RED);
				if (tileType == Tile::Floor)
					DrawCircle(GetScreenPositionOfTile(coordinatesOfTile).x + addForCenterX, GetScreenPositionOfTile(coordinatesOfTile).y + addForCenterY, 10, GREEN);

				if (tileType == Tile::Floor)
				{
					std::vector<Vector2> traversibleAdjacentTiles = GetTraversibleTilesAdjacentTo(vec2CoordinatesOfTile);
					if (traversibleAdjacentTiles.size() > 0)
					{
						for (int i = 0; i < traversibleAdjacentTiles.size(); i++)
						{
							Vector2 screenPosOfTraversibleAdjacentTile = TilePosToScreenPos(traversibleAdjacentTiles[i]);
							DrawLine(GetScreenPositionOfTile(coordinatesOfTile).x + addForCenterX, GetScreenPositionOfTile(coordinatesOfTile).y + addForCenterY, screenPosOfTraversibleAdjacentTile.x + addForCenterX, screenPosOfTraversibleAdjacentTile.y + addForCenterY, GREEN);
						}
					}
				}
			}
		}
	}

};