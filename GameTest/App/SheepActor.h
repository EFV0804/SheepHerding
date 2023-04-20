#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BoundingBoxComponent.h"
#include <chrono>

class CSheepActor :
    public CActor
{
public:
    CSheepActor(CVec2 position);
    CBoundingBoxComponent* GetBoundingBox() { return m_bb; }
    void Render();
    void UpdateActor();
    void DrawBoundingBox();
    CVec2 m_velocity{ 0.0f,0.0f };
    bool m_isGrazing = false;
    bool m_isActive = true;

    std::chrono::steady_clock::time_point m_startGrazeTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_currentGrazeTime = std::chrono::steady_clock::now();
    float m_grazeCoolDown = 2.0f;

private:
    CSpriteComponent* m_sprite = nullptr;
    CBoundingBoxComponent* m_bb = nullptr;
};
