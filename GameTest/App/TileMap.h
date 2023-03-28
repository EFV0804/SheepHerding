#pragma once
class CTileMap
{
public:
	CTileMap() = delete;
	const CTileMap& operator=(const CTileMap&) = delete;
	CTileMap(const CTileMap&) = delete;

	void Load();
	int GetTile();

};

