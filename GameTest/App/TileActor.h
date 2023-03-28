#pragma once
#include "Actor.h"
#include "SpriteComponent.h"

class CTileActor :
	public CActor {

public:
	CTileActor(CVec2 position, int frame);

private:
	CSpriteComponent* m_sprite;
};