#include "stdafx.h"
#include "Component.h"
#include "Actor.h"

CComponent::CComponent(CActor* owner, int updateOrder, bool isRenderable): 
	m_owner(*owner), m_updateOrder (updateOrder), m_isRenderable(isRenderable)
{
	m_owner.AddComponent(this);
}

CComponent::~CComponent()
{
	m_owner.RemoveComponent(this);
}

void CComponent::Update(float deltaTime)
{

}

void CComponent::Render()
{

}
