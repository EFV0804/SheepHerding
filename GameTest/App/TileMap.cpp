#include "stdafx.h"
#include "TileMap.h"
#include <string>
#include <iostream>
#include <fstream>

void CTileMap::Load()
{
	int mapNum = 1+rand() % 2;

	char buff[100];
	snprintf(buff, sizeof(buff), ".\\TestData\\map_%d.txt", mapNum);
	
	ReadFromFile(buff);
//
//	m_tilemap = {
//		0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//;		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,2,0,0,0,0,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,2,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,2,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,2,0,0,0,0,0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//		0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
//
//	}

	if (m_tiles.size() > 0) {
		for (auto tile : m_tiles) {
			delete tile;
		}
		m_tiles.clear();
	}

	for (int row_it = m_rows; row_it--> 0;) {
		for (int col_it = 0; col_it < m_cols; col_it++) 
		{

			CVec2 position{ 0.0f,0.0f };
			position.m_y = ((m_rows-1) - row_it) * TILE_SIZE + TILE_SIZE * 0.5;
			position.m_x = col_it * TILE_SIZE + TILE_SIZE * 0.5;

			TileType type;

			switch (m_tilemap[row_it * m_cols + col_it]) {
				default:
					type = TileType::Grass;
				case 0:
					type = TileType::Mud;
					break;
				case 1:
					type = TileType::Grass;
					break;
				case 2:
					type = TileType::Fence;
					break;
			}

			m_tiles.emplace_back(new CTileActor(position, type));

			//CTileActor* tile = new CTileActor(position, type);
		}
	}
}

void CTileMap::ReadFromFile(const char* filename)
{
	std::string line;
	std::ifstream mapData;
	mapData.open(filename);

	for (int row_it = 0; row_it < m_rows; row_it++) {
		for (int col_it = 0; col_it < m_cols; col_it++) {
			std::getline(mapData, line, ',');
			m_tilemap[row_it * m_cols + col_it] = std::stoi(line);
		}
	}



}

int CTileMap::GetTileType(CVec2 position)
{
	int row{ 30 };
	int col{ 0 };
	//CVec2 temp_position{ 65.0f,0.1f };
	if (position.m_x < APP_VIRTUAL_WIDTH && position.m_x > 0 && position.m_y < APP_VIRTUAL_HEIGHT && position.m_y > 0) {
		col = floor(position.m_x / TILE_SIZE);
		row = floor(position.m_y / TILE_SIZE);
		return m_tilemap[row * m_cols + col];
	}
	else {
		return -1;
	}


}

