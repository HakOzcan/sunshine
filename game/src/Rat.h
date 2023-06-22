#pragma once
#include "Tilemap.h"

class Rat
{
public:
	Vector2 ratPos;
	Tilemap tileMap;
	float ratScreenPosX;
	float ratScreenPosY;

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
			bool canRatGoN = this->tileMap.IsTraversible(ratPos);
			if (canRatGoN)
			{
				this->ratPos = this->tileMap.NORTH;
				this->ratScreenPosY -= this->tileMap.tileSizeY;
			}
		}
		if (IsKeyPressed(KEY_A))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.WEST;
			bool canRatGoW = this->tileMap.IsTraversible(ratPos);
			if (canRatGoW)
			{
				this->ratPos = this->tileMap.WEST;
				this->ratScreenPosX -= this->tileMap.tileSizeX;
			}
		}
		if (IsKeyPressed(KEY_S))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.SOUTH;
			bool canRatGoS = this->tileMap.IsTraversible(ratPos);
			if (canRatGoS)
			{
				this->ratPos = this->tileMap.SOUTH;
				this->ratScreenPosX += this->tileMap.tileSizeX;
			}
		}
		if (IsKeyPressed(KEY_D))
		{
			Vector2 tempPos = this->ratPos + this->tileMap.EAST;
			bool canRatGoE = this->tileMap.IsTraversible(ratPos);
			if (canRatGoE)
			{
				this->ratPos = this->tileMap.EAST;
				this->ratScreenPosY += this->tileMap.tileSizeY;
			}
		}
	}
};
