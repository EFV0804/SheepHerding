#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BoundingBoxComponent.h"
#include "CollisionComponent.h"
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
    CSpriteComponent* m_sprite;
    CBoundingBoxComponent* m_bb;
    CInputComponent* m_inputs;
    //CCollisionComponent* m_collider;
};

