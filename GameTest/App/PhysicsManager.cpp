#include "stdafx.h"
#include "PhysicsManager.h"
#include <algorithm>
#include "app.h"

CPhysicsManager::CPhysicsManager(): m_compare(0)
{

}

void CPhysicsManager::Update(float deltaTime)
{
}

CVec2 CPhysicsManager::GetPredictedPosition(CActor* actor, CVec2& force, float deltaTime)
{
	CVec2 translate{ 0.0f,0.0f };
	translate = actor->GetPosition() + force * deltaTime;
	return translate;
}

bool CPhysicsManager::AABBCollision(CBoundingBoxComponent* a, CBoundingBoxComponent* b)
{

	float aScale = a->GetScale();

	CVec2 positionA = a->GetPosition();
	float aMinX = positionA.m_x;
	float aMinY = positionA.m_y;

	aMinX = (aMinX - ((a->GetSize().m_x / 4.0f) * aScale));
	aMinY = (aMinY - ((a->GetSize().m_y / 4.0f) * aScale));
	float aMaxX = (aMinX + ((a->GetSize().m_x / 2.0f) * aScale));
	float aMaxY = (aMinY + ((a->GetSize().m_y / 2.0f) * aScale));


	float bScale = b->GetScale();
	CVec2 positionB = b->GetPosition();
	float bMinX = positionB.m_x;
	float bMinY = positionB.m_y;

	bMinX = (bMinX - ((b->GetSize().m_x / 4.0f) * bScale));
	bMinY = (bMinY - ((b->GetSize().m_y / 4.0f) * bScale));
	float bMaxX = (bMinX + ((b->GetSize().m_x / 2.0f) * bScale));
	float bMaxY = (bMinY + ((b->GetSize().m_y / 2.0f) * bScale));

	return !(aMinX > bMaxX || aMaxX < bMinX || aMinY > bMaxY || aMaxY < bMinY);

}

void CPhysicsManager::UpdateActorPosition(CBoundingBoxComponent* bb, CVec2& force, float deltaTime)
{
	CVec2 translate{ 0.0f,0.0f };
	translate = bb->GetPosition() + force * deltaTime;
	bb->SetPosition(translate);
}


void CPhysicsManager::AddBody(CBoundingBoxComponent* bb)
{
	m_bodies.push_back(bb);
}
