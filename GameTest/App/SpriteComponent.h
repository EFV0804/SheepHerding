#pragma once
#include "Component.h"
#include "SimpleSprite.h"
#include "Vec2.h"

class CSpriteComponent :
    public CComponent
{
public:
    CSpriteComponent(CActor* owner,
        int updateOrder,
        const char* fileName = ".\\TestData\\default.bmp",
        unsigned int nColumns = 1,
        unsigned int nRows = 1);
    CSpriteComponent(const CSpriteComponent&) = delete;
    CSpriteComponent() = delete;
    CSpriteComponent& operator=(const CSpriteComponent&) = delete;

    void Update(float deltaTime);
    void Render();

    CSimpleSprite* GetSprite() { return m_sprite.get(); }
    void SetPosition(CVec2 position) { m_position = position; }

    enum
    {
        ANIM_FORWARDS,
        ANIM_BACKWARDS,
        ANIM_LEFT,
        ANIM_RIGHT,
    };

private:
    CVec2 m_position{ 0.0f,0.0f };
    const char* m_fileName;
    std::unique_ptr<CSimpleSprite> m_sprite;
    unsigned int m_nColumns;
    unsigned int m_nRows;
};

