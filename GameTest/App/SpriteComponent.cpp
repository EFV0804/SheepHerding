#include "stdafx.h"
#include "SpriteComponent.h"
#include "Vec2.h"
#include "Actor.h"

CSpriteComponent::CSpriteComponent(CActor* owner, 
    int updateOrder,
    const char* fileName,
    unsigned int nColumns,
    unsigned int nRows) : CComponent(owner, updateOrder, true),
    m_fileName(fileName),
    m_nColumns(nColumns),
    m_nRows(nRows)
{
    m_sprite = std::make_unique<CSimpleSprite>(fileName, nColumns, nRows);
    m_sprite->SetFrame(0);
}

void CSpriteComponent::Update(float deltaTime)
{
    CVec2 position{ 0.0f,0.0f };
    m_owner.GetPosition(position);
    m_sprite->SetPosition(position.m_x, position.m_y);
    float scale = m_owner.GetScale();
    m_sprite->SetScale(scale);
	m_sprite->Update(deltaTime);
}

void CSpriteComponent::Render()
{
	m_sprite->Draw();
}
