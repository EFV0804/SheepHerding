#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BoundingBoxComponent.h"
#include "InputComponent.h"
#include <chrono>

class CPlayerActor :
    public CActor
{
public:

    CPlayerActor(CVec2 position, int order=3);
    void Render() override;
    void DrawBoundingBox();
    void UpdateActor(float deltaTime);
    void ButtonAction(int button) override;
    CBoundingBoxComponent* GetBoundingBox() { return m_bb; }

private:
    enum
    {
        ANIM_BARK,
    };
    CSpriteComponent* m_sprite = nullptr;
    //CSpriteComponent* m_barkSprite = nullptr;
    CBoundingBoxComponent* m_bb = nullptr;
    CInputComponent* m_inputs = nullptr;
    int m_baseForce{ 200 };
    int m_forceBoost{ 200 };
    std::chrono::steady_clock::time_point m_startBarkTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_currentBarkTime = std::chrono::steady_clock::now();
    bool m_hasBarked = false;
    float m_barkCoolDown = 0.2f;
    int m_updateOrder{ 3 };
};

