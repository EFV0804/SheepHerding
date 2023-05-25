#include "stdafx.h"
#include "PhysicsManager.h"
#include <algorithm>
#include "app.h"
#include <array>

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

void CPhysicsManager::WallCollision(CActor* actor, CVec2& force, float deltaTime)
{
	std::array<CVec2, 4> dirs = {
		CVec2{ 1.0f,0.0f },CVec2{-1.0f,0.0f},CVec2{0.0f,1.0f},CVec2{0.0f,-1.0f}

	};

	for(auto body : m_staticBodies){

		if (AABBCollision(body, actor->GetBoundingBox())) {

			CVec2 centerBody = body->GetCenter();
			CVec2 vecToActor = actor->GetPosition() - centerBody;
			vecToActor.Normalised();
			
			CVec2 closestDir{0.0f,0.0f};
			float highestDot{-1.0f };

			for (int i = 0; i < dirs.size(); i++) {
				float dot = CVec2::dot(vecToActor, dirs[i]);
				if (dot > highestDot) {
					highestDot = dot;
					closestDir = dirs[i];
				}
			}
			 
			if (closestDir.m_x != 0) {
				force.m_x *= -1;
			}
			else if (closestDir.m_y != 0) {
				force.m_y *= -1;
			}
		}
	}
}

bool CPhysicsManager::AABBCollision(const CBoundingBoxComponent* a, const CBoundingBoxComponent* b)
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

bool CPhysicsManager::RaycastIntersects(CVec2 rayPos, CVec2 rayDir, CBoundingBoxComponent* bb)
{



	float xmin = bb->GetPosition().m_x;
	float ymin = bb->GetPosition().m_y;
	xmin = (xmin - ((bb->GetSize().m_x / 4.0f) * bb->GetScale()));
	ymin = (ymin - ((bb->GetSize().m_y / 4.0f) * bb->GetScale()));
	float xmax = (xmin + ((bb->GetSize().m_x / 2.0f) * bb->GetScale()));
	float ymax = (ymin + ((bb->GetSize().m_y / 2.0f) * bb->GetScale()));

	std::vector<std::array<CVec2,2>> segments;
	std::array<CVec2, 2> arr{ CVec2{0.0f,0.0f},CVec2{0.f,0.f} };

	CVec2 segment1A{ xmin, ymax };
	CVec2 segment1B = { xmax, ymax };
	arr[0] = segment1A;
	arr[1] = segment1B;
	segments.push_back(arr);

	CVec2 segment2A{ xmin, ymin };
	CVec2 segment2B = { xmax, ymin };
	arr[0] = segment2A;
	arr[1] = segment2B;
	segments.push_back(arr);

	CVec2 segment3A{ xmax, ymax };
	CVec2 segment3B = { xmax, ymin };
	arr[0] = segment3A;
	arr[1] = segment3B;
	segments.push_back(arr);

	CVec2 segment4A{ xmin, ymax };
	CVec2 segment4B = { xmin, ymin };
	arr[0] = segment4A;
	arr[1] = segment4B;
	segments.push_back(arr);

	float x3 = rayPos.m_x;
	float y3 = rayPos.m_y;
	CVec2 norm = CVec2::Normalised(rayDir);
	float x4 = rayPos.m_x + rayDir.m_x* norm.m_x;
	float y4 = rayPos.m_y + rayDir.m_y * norm.m_y;

	for (auto segment : segments) {
		float den = (segment[0].m_x - segment[1].m_x) * (y3 - y4) - (segment[0].m_y - y3) * (x3 - x4);

		if (den == 0) {
			return false;
		}
		float t = (segment[0].m_x - x3) * (y3 - y4) - (segment[0].m_y - y3) * (x3 - x4) / den;
		float u = -(segment[0].m_x - segment[1].m_x) * (segment[0].m_y - y3) - (segment[0].m_y - segment[1].m_x) * (segment[0].m_x - x3) / den;

		if (t > 0 && t < 1 && u > 0) {
			return true;
		}
		else {
			return false;
		}
	}

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
	if (actor->GetBoundingBox() != nullptr) {
		//WallCollision(actor, force, deltaTime);
	}


	CVec2 translate{ 0.0f,0.0f };
	translate = actor->GetPosition() + force * deltaTime;
	actor->SetPosition(translate);
}

void CPhysicsManager::AddBody(CBoundingBoxComponent* bb)
{
	if (bb->m_isStatic) {
		m_staticBodies.push_back(bb);
	}
	else {
		m_dynamicBodies.push_back(bb);

	}

}

void CPhysicsManager::RemoveBody(CBoundingBoxComponent* bb)
{
	if (bb->m_isStatic) {
		auto iter = std::find(begin(m_staticBodies), end(m_staticBodies), bb);
		if (iter != end(m_staticBodies)) {
			m_staticBodies.erase(iter);
		}
	}
	else {
		auto iter = std::find(begin(m_dynamicBodies), end(m_dynamicBodies), bb);
		if (iter != end(m_dynamicBodies)) {
			m_dynamicBodies.erase(iter);
		}
	}

}
