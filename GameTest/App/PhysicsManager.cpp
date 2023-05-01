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

void CPhysicsManager::UpdateActorPosition(CActor* actor, CVec2& force, float deltaTime)
{
	int xMin = 0;
	int yMin = 0;
	int xMax = APP_VIRTUAL_WIDTH;
	int yMax = APP_VIRTUAL_HEIGHT;

	CVec2 vec{ 0.0f,0.0f };
	CVec2 predictedPos = CPhysicsManager::Get().GetPredictedPosition(actor, force * 2.0f, deltaTime);


	if (predictedPos.m_x <= xMin) {
		force = force * CVec2{ -1.0f,1.0f };
	}
	else if (predictedPos.m_x >= xMax) {
		force = force * CVec2{ -1.0f,1.0f };
	}
	if (predictedPos.m_y <= yMin) {
		force = force * CVec2{ 1.0f,-1.0f };
	}
	else if (predictedPos.m_y >= yMax) {
		force = force * CVec2{ 1.0f,-1.0f };

	}

	CVec2 translate{ 0.0f,0.0f };
	translate = actor->GetPosition() + force * deltaTime;
	actor->SetPosition(translate);
}


void CPhysicsManager::AddBody(CBoundingBoxComponent* bb)
{
	m_bodies.push_back(bb);
}
