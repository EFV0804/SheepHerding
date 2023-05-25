#include "stdafx.h"
#include "TileMap.h"
#include <string>
#include <iostream>
#include <fstream>
#include <random>

void CTileMap::Load()
{
	if (m_tiles.size() > 0) {
		m_tiles.clear();
		m_grassTiles.clear();
		m_enclosureTiles.clear();
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, 7); // define the range

	int mapNum = distr(gen);

	char buff[21];
	snprintf(buff, sizeof(buff), ".\\TestData\\map_%d.txt", mapNum);
	
	ReadFromFile(buff);

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
			position.m_y = ((m_rows - 1) - row_it) * TILE_SIZE + TILE_SIZE * 0.5;
			position.m_x = col_it * TILE_SIZE + TILE_SIZE * 0.5;

			TileType type;

			switch (m_tilemap[row_it * m_cols + col_it]) {
				default:
					type = TileType::Grass;
					m_tiles.emplace_back(new CTileActor(position, type));
					//m_grassTiles.push_back(m_tiles.back());
				case 0:
					type = TileType::Mud;
					m_tiles.emplace_back(new CTileActor(position, type));
					m_enclosureTiles.push_back(m_tiles.back());
					break;
				case 1:
					type = TileType::Grass;
					m_tiles.emplace_back(new CTileActor(position, type));
					//m_grassTiles.push_back(m_tiles.back());

					break;
				case 2:
					type = TileType::Fence;
					m_tiles.emplace_back(new CTileActor(position, type));
					m_enclosureTiles.push_back(m_tiles.back());
					break;
			}



		}
	}
	int x = 0;
	//Get elligible grass tile for sheep p
	for (auto tile : m_tiles) {
		if (tile->GetType() == 1
			&& tile->GetPosition().m_y < 800
			&& tile->GetPosition().m_y > 100
			&& tile->GetPosition().m_x < 800
			&& tile->GetPosition().m_x > 100) {

			m_grassTiles.push_back(tile);
		}
	}
	ComputeAverageEnclosurePosition();
}

void CTileMap::ReadFromFile(const char* filename)
{
	std::string line;
	std::ifstream mapData;
	//mapData.open(".\\TestData\\map_template.txt");
	mapData.open(filename);

	for (int row_it = 0; row_it < m_rows; row_it++) {
		for (int col_it = 0; col_it < m_cols; col_it++) {
			std::getline(mapData, line, ',');
			m_tilemap[row_it * m_cols + col_it] = std::stoi(line);
		}
	}
}

void CTileMap::ComputeAverageEnclosurePosition()
{

	for (auto tile : m_enclosureTiles) {
		m_averageEnclosurePos.m_x += tile->GetPosition().m_x;
		m_averageEnclosurePos.m_y += tile->GetPosition().m_y;
	}

	m_averageEnclosurePos.m_x = m_averageEnclosurePos.m_x / m_enclosureTiles.size();
	m_averageEnclosurePos.m_y = m_averageEnclosurePos.m_y / m_enclosureTiles.size();

}

int CTileMap::GetTileType(CVec2 position)
{
	int row{ 0 };
	int col{ 0 };

	if (position.m_x < APP_VIRTUAL_WIDTH && position.m_x > 0 && position.m_y < APP_VIRTUAL_HEIGHT && position.m_y > 0) {
		col = floor(position.m_x / TILE_SIZE);
		row = floor(position.m_y / TILE_SIZE);

		return m_tiles.at(row * m_cols + col)->GetType();
	}
	else {
		return -1;
	}


}

