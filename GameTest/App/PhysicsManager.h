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
	CPhysicsManager(const CPhysicsManager&) = delete;
	CPhysicsManager& operator=(const CPhysicsManager&) = delete;

	static CPhysicsManager& Get() {
		static CPhysicsManager instance;
		return instance;
	}

	void Update(float deltaTime);
	CVec2 GetPredictedPosition(CActor* actor, CVec2& force, float deltaTime);
	void WallCollision(CActor* actor, CVec2& force, float deltaTime);
	bool AABBCollision(const CBoundingBoxComponent* a, const CBoundingBoxComponent* b);
	void UpdateActorPosition(CActor* actor, CVec2& translate, float deltaTime);
	void AddBody(CBoundingBoxComponent* bb);
	void RemoveBody(CBoundingBoxComponent* bb);

private:
	std::vector<CBoundingBoxComponent*> m_staticBodies;
	std::vector<CBoundingBoxComponent*> m_dynamicBodies;
	CCompareAABB m_compare;
};