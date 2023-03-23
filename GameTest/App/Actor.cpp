#include "stdafx.h"
#include "Actor.h"

void CActor::Update(float deltaTime)
{
	//m_sprite->SetPosition(m_position.GetX(), m_position.GetY());
	m_sprite->Update(deltaTime);
}

void CActor::Draw()
{
	m_sprite->Draw();
}

void CActor::SetSprite(CSimpleSprite* sprite)
{
	m_sprite = sprite;
	m_sprite->SetPosition(m_position.GetX(), m_position.GetY());
}
