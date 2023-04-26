#pragma once
#include <array>
#include "Vec2.h"
#include "TileActor.h"

const int TILE_SIZE = 64;


class CTileMap
{
public:
	CTileMap() = default;
	const CTileMap& operator=(const CTileMap&) = delete;
	CTileMap(const CTileMap&) = delete;

	static CTileMap& Get() {
		static CTileMap instance;
		return instance;
	}

	void Load();
	void ReadFromFile(const char* filename);
	int GetTileType(CVec2 position);
	const int m_rows = 17;
	const int m_cols = 30;
	int m_totalTiles = 510;

private:
	std::vector<CTileActor*> m_tiles;
	std::array<int, 510> m_tilemap;
};

