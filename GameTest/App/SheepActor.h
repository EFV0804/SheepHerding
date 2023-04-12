#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BoundingBoxComponent.h"

class CSheepActor :
    public CActor
{
public:
    CSheepActor(CVec2 position);
    CBoundingBoxComponent* GetBoundingBox() { return m_bb; }
    void Render();
    void UpdateActor();
    void DrawBoundingBox();

private:
    CSpriteComponent* m_sprite = nullptr;
    CBoundingBoxComponent* m_bb = nullptr;
};
