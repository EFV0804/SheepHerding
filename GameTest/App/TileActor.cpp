#include "stdafx.h"
#include "TileActor.h"

CTileActor::CTileActor(CVec2 position, int frame): CActor(position)
{
	m_sprite = new CSpriteComponent(this, 1, ".\\TestData\\tiles.bmp", 3, 1);
	m_sprite->GetSprite()->SetFrame(frame);
}
