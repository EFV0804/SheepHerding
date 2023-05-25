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
	void ComputeAverageEnclosurePosition();

	int GetTileType(CVec2 position);
	const std::vector<CTileActor*>& GetTiles() const { return m_tiles; }
	std::vector<CTileActor*>& GetGrassTiles() { return m_grassTiles; }
	const std::vector<CTileActor*> GetEnclosureTiles() const { return m_enclosureTiles; }
	const CVec2 GetAverageEnclosurePos() const { return m_averageEnclosurePos; }

	const int m_rows = 17;
	const int m_cols = 30;
	int m_totalTiles = 510;

private:
	std::vector<CTileActor*> m_tiles;
	std::vector<CTileActor*> m_grassTiles;
	std::vector<CTileActor*> m_enclosureTiles;
	CVec2 m_averageEnclosurePos{ 0.0f,0.0f };
	std::array<int, 510> m_tilemap;
};

