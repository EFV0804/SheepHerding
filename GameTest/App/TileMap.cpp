#include "stdafx.h"
#include "TileMap.h"
#include "TileActor.h"
#include <array>
#include <iostream>
#include "app.h"

void CTileMap::Load()
{
	const int rows = 12;
	const int cols = 16;
	const int totalTiles = rows * cols;
	int tileSize = 64;

	std::array<int, totalTiles> tilemap = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,
		1,1,1,2,0,0,0,0,0,0,0,2,1,1,1,1,
		1,1,1,2,0,0,0,0,0,0,0,0,1,1,1,1,
		1,1,1,2,0,0,0,0,0,0,0,0,1,1,1,1,
		1,1,1,2,0,0,0,0,0,0,0,2,1,1,1,1,
		1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			CVec2 position{ 0.0f,0.0f };
			position.m_y = i * tileSize + tileSize * 0.5;
			position.m_x = j * tileSize + tileSize * 0.5;
			
			CTileActor* tile = new CTileActor(position, tilemap[i * cols + j]);
		}
	}
}

