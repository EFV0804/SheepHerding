#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BoundingBoxComponent.h"
#include "InputComponent.h"

class CPlayerActor :
    public CActor
{
public:

    CPlayerActor(CVec2 position);
    void Render();
    void DrawBoundingBox();
    void UpdateActor(float deltaTime);

    CBoundingBoxComponent* GetBoundingBox() { return m_bb; }

private:
    CSpriteComponent* m_sprite = nullptr;
    CBoundingBoxComponent* m_bb = nullptr;
    CInputComponent* m_inputs = nullptr;
};

