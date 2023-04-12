#include "stdafx.h"
#include "PhysicsManager.h"
#include <algorithm>

CPhysicsManager::CPhysicsManager(): compare(0)
{

}

void CPhysicsManager::Update(float deltaTime)
{
	std::sort(m_bodies.rbegin(), m_bodies.rend(), [](const CBoundingBoxComponent* a, const CBoundingBoxComponent* b) {
		return a->GetForce() < b->GetForce(); });

	for (int i = 0; i < m_bodies.size(); i++) {

		CBoundingBoxComponent* AABB_1 = m_bodies.at(i);

		for (int j = i+1; j < m_bodies.size(); j++) {
			CBoundingBoxComponent* AABB_2 = m_bodies.at(j);

			CVec2 distanceVec = AABB_1->GetPosition() - AABB_2->GetPosition();
			float distance = distanceVec.Length();
			if (distance < AABB_1->GetForce()) {
				CVec2 dir = AABB_2->GetPosition() - AABB_1->GetPosition();
				dir.Normalised();
				float repelStrength = (20.0f / distance) * AABB_1->GetForce();
				CVec2 force = repelStrength * dir;
				AABB_2->SetPosition(AABB_2->GetPosition() + force * (0.002*deltaTime));
			}
		}
	}



	// COLLISION ALL NO SWEEP AND SORT

	//for (int i = 0; i < m_bodies.size(); i++) {

	//	CBoundingBoxComponent* AABB_1 = m_bodies.at(i);

	//	for (int j = i+1; j < m_bodies.size(); j++) {
	//		CBoundingBoxComponent* AABB_2 = m_bodies.at(j);

	//		if (AABBCollision(AABB_2, AABB_1)) {
	//			CVec2 direction = AABB_2->GetPosition() - AABB_1->GetPosition();
	//			direction.Normalised();
	//			AABB_2->SetPosition(AABB_2->GetPosition() + direction * deltaTime);
	//		}

	//	}
	//}


	//COLLISION ALL SWEEP AND SORT, BROKEN

	////sort m_bodies by min axis position
	//std::sort(m_bodies.begin(), m_bodies.end(), compare);


	////CBoundingBoxComponent* currentBody = m_bodies.at(0);

	//// Update position
	//for (int i = 0; i < m_bodies.size(); i++) {

	//	CBoundingBoxComponent* AABB_1 = m_bodies.at(i);
	//	//CVec2 centerSum(0.0,0.0);
	//	//CVec2 centerSqrSum(0.0, 0.0);
	//	//centerSum += AABB_1->GetCenter();
	//	//centerSqrSum += AABB_1->GetCenter() * AABB_1->GetCenter();

	//	for (int j = i + 1; j < m_bodies.size(); j++) {
	//		CBoundingBoxComponent* AABB_2 = m_bodies.at(j);

	//		//ADD EARLY BREAK IF BODIES SORTED
	//		if (AABB_2->GetMinExtents()[0] > AABB_1->GetMaxExtents()[0]) {
	//			break;
	//		}
	//		if (AABBCollision(AABB_2, AABB_1)) {
	//			// The sprite whose postion is set is determined by it's original position in x, so when dog
	//			// comes from left, it gets pushed instead of sheep
	//			// Do agression check to determine who gets pushed
	//			std::vector<CBoundingBoxComponent*> forceVec{ AABB_1, AABB_2 };
	//			std::sort(forceVec.begin(), forceVec.end(), [](const CBoundingBoxComponent* a, const CBoundingBoxComponent* b) {
	//				return a->GetForce() < b->GetForce();
	//				});
	//			CVec2 direction = forceVec.at(0)->GetPosition() - forceVec.at(1)->GetPosition();
	//			direction.Normalised();
	//			forceVec.at(0)->SetPosition(forceVec.at(0)->GetPosition() + direction * deltaTime);

	//		}
	//	}
	//	//currentBody = m_bodies.at(i);
	//}

}

bool CPhysicsManager::AABBCollision(CBoundingBoxComponent* a, CBoundingBoxComponent* b)
{

	float aScale = a->GetScale();

	CVec2 positionA = a->GetPosition();
	float aMinX = positionA.m_x;
	float aMinY = positionA.m_y;

	//a->GetPosition(aMinX, aMinY);

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

void CPhysicsManager::AddBody(CBoundingBoxComponent* bb)
{
	m_bodies.push_back(bb);
}
