#pragma once
#include "Tilemap.h"

class Rat
{
public:
	Vector2 ratPos;
	Tilemap tileMap;
	float ratScreenPosX;
	float ratScreenPosY;
	Tile tiles[10][10];

	Rat()
	{

	}

	void SpawnRat()
	{
		this->ratPos.x = rand() % this->tileMap.GetGridWidth();
		this->ratPos.y = rand() % this->tileMap.GetGridHeight();
		this->ratScreenPosX = this->tileMap.TilePosToScreenPos(ratPos).x;
		this->ratScreenPosY = this->tileMap.TilePosToScreenPos(ratPos).y;
	}

	void Update()
	{
		if (IsKeyPressed(KEY_W))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.NORTH;
			bool isInside = this->tileMap.IsInsideGrid(ratPos.x, ratPos.y);
			//bool isItFloor = this->tileMap.IsItFloor(ratPos.x, ratPos.y);
			if (isInside)
			{
				this->ratPos = this->tileMap.NORTH;
				this->ratScreenPosY -= this->tileMap.tileSizeY;
			}
		}
		if (IsKeyPressed(KEY_A))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.WEST;
			bool canRatGoW = this->tileMap.IsTraversible(ratPos);
			if (!canRatGoW)
			{
				this->ratPos = this->tileMap.WEST;
				this->ratScreenPosX -= this->tileMap.tileSizeX;
			}
		}
		if (IsKeyPressed(KEY_S))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.SOUTH;
			bool canRatGoS = this->tileMap.IsTraversible(ratPos);
			if (!canRatGoS)
			{
				this->ratPos = this->tileMap.SOUTH;
				this->ratScreenPosY += this->tileMap.tileSizeY;
			}
		}
		if (IsKeyPressed(KEY_D))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.EAST;
			bool canRatGoE = this->tileMap.IsTraversible(ratPos);
			if (!canRatGoE)
			{
				this->ratPos = this->tileMap.EAST;
				this->ratScreenPosX += this->tileMap.tileSizeX;
			}
		}
	}
};
