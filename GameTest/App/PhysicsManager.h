#pragma once
#include <vector>
#include "BoundingBoxComponent.h"

struct CCompareAABB {
	CCompareAABB(int axis):m_axis(axis) {}
	int m_axis;
	bool operator()(CBoundingBoxComponent* a, CBoundingBoxComponent* b) {
		float aMin = a->GetMinExtents()[m_axis];
		float bMin = b->GetMinExtents()[m_axis];

		return aMin < bMin;
	}
};

class CPhysicsManager {
public:
	CPhysicsManager();
	void Update(float deltaTime);

	bool AABBCollision(CBoundingBoxComponent* a, CBoundingBoxComponent* b);
	void AddBody(CBoundingBoxComponent* bb);

private:
	std::vector<CBoundingBoxComponent*> m_bodies;
	CCompareAABB compare;

};