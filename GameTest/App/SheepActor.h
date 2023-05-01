#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BoundingBoxComponent.h"
#include <chrono>

class CSheepActor :
    public CActor
{
public:
    CSheepActor(CVec2 position, int order=2);
    CBoundingBoxComponent* GetBoundingBox() { return m_bb; }

    void Render();
    void UpdateActor();
    void DrawBoundingBox();
    bool CanGraze();
    const int GetOrder() const { return m_updateOrder; }

    bool m_isGrazing = false;
    bool m_isActive = true;

    std::chrono::steady_clock::time_point m_startGrazeTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_currentGrazeTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_startGrazeSubmitTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_currentGrazeSubmitTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> m_grazeWait{ 2.0 };
    std::chrono::duration<double> m_grazeCoolDown{ 2.0 };

    CSpriteComponent* GetSprite() { return m_sprite; }

private:
    CSpriteComponent* m_sprite = nullptr;
    CBoundingBoxComponent* m_bb = nullptr;
    int m_updateOrder{ 2 };
};
