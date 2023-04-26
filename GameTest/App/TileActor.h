#pragma once
#include "Actor.h"
#include "SpriteComponent.h"

enum TileType {
	Mud,
	Grass,
	Fence
};

class CTileActor :
	public CActor {

public:
	CTileActor() = delete;
	CTileActor(CVec2 position, TileType type, int order = 1);

private:
	TileType m_type;
	CSpriteComponent* m_sprite = nullptr;
	int m_updateOrder{ 1 };

};