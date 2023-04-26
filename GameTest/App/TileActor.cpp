#include "stdafx.h"
#include "TileActor.h"

CTileActor::CTileActor(CVec2 position, TileType type, int order): CActor(position, order), m_type(type)
{
	m_sprite = new CSpriteComponent(this, 1, ".\\TestData\\tiles.bmp", 3, 1);
	m_sprite->GetSprite()->SetFrame(type);
}
