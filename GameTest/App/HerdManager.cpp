#include "stdafx.h"
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include "HerdManager.h"
#include "PhysicsManager.h"
#include "TileMap.h"
#include "Math.h"

constexpr float centeringFactor{ 0.2f };
constexpr float avoidanceFactor{0.5f };
constexpr float cohesionFactor{ 2.5f };
constexpr int grazingChance{ 5 };

CHerdManager::CHerdManager()
{

}

void CHerdManager::MakeHerd(int sheepCount)
{
	m_sheepCount = sheepCount;
	srand(time(0));

	for (int i = 0; i < m_sheepCount; i++) {

		float x = 1 + rand() % 900;
		float y = 1 + rand() % 600;
		CVec2 randPos{x,y};

		m_herd.push_back(new CSheepActor(randPos));
	}
}

void CHerdManager::ComputeDog(float deltaTime)
{
	for (auto sheep : m_herd) {
		CVec2 distanceVec = m_dog->GetPosition() - sheep->GetPosition();
		float distance = distanceVec.Length();
		if (distance < m_dog->GetForce()) {
			CVec2 dir = sheep->GetPosition() - m_dog->GetPosition();
			dir.Normalised();
			float repelStrength =  m_dog->GetForce() - distance;
			CVec2 force = repelStrength * dir;
			CPhysicsManager::Get().UpdateActorPosition(sheep->GetBoundingBox(), force*0.002f, deltaTime);
		}
	}
}

void CHerdManager::ComputeSheep(float deltaTime)
{
	ComputePosition();

	for (auto sheep : m_herd) {
		//DetectEnclosure(sheep);

		if (sheep->m_isActive) {

			//SetIsGrazing(sheep);

			if (!sheep->m_isGrazing) {
				//CVec2 centerVec = MoveToCenter(sheep);
				//sheep->m_velocity += centerVec;
				//CVec2 avoidanceVec = AvoidOthers(sheep);
				//sheep->m_velocity += avoidanceVec;
				CVec2 velocityVec = MatchVelocity(sheep);
				sheep->m_velocity += velocityVec;
				CVec2 boundVelocity = BindPosition(sheep, deltaTime);
				sheep->m_velocity += boundVelocity;

			}

			LimitVelocity(sheep);
			//RandomiseVelocity(sheep);

			CPhysicsManager::Get().UpdateActorPosition(sheep->GetBoundingBox(), sheep->m_velocity, deltaTime);

			//sheep->m_velocity *= 0.5f;

		}
	}
}

void CHerdManager::Update(float deltaTime)
{
	ComputeDog(deltaTime);
	ComputeSheep(deltaTime);
}

void CHerdManager::ComputePosition()
{
	float x{ 0.0f };
	float y{ 0.0f };

	for (auto sheep : m_herd) {
		x += sheep->GetPosition().m_x;
		y += sheep->GetPosition().m_y;
	}
	m_position = CVec2(x / m_herd.size(), y / m_herd.size());
}

CVec2 CHerdManager::MoveToCenter(CSheepActor* sheep)
{
	//ComputePosition();

	CVec2 vecToHerd = m_position - sheep->GetPosition();
	float distanceToHerd = vecToHerd.Length();

	vecToHerd.Normalised();

	if (distanceToHerd > m_radius) {

		return vecToHerd * centeringFactor;
	}
	else {
		return CVec2(0.0f, 0.0f);
	}


	//}

	/*CVec2 center{ 0.0f, 0.0f };
	int count{ 0 };

	for (auto otherSheep : m_herd) {
		if (otherSheep != sheep) {
			center += otherSheep->GetPosition();
			count++;
		}
	}

	center /= count;
	CVec2 vecToHerd = (center - sheep->GetPosition()) / centeringFactor;

	return vecToHerd;*/

}

CVec2 CHerdManager::AvoidOthers(CSheepActor* sheep)
{
	CVec2 center{ 0.0f,0.0f };

	for (auto otherSheep : m_herd) {
		if (otherSheep != sheep) {
			CVec2 vecToSheep = sheep->GetPosition() - otherSheep->GetPosition();
			float distanceBetweenSheep = vecToSheep.Length();

			if (distanceBetweenSheep < 50.0f) {
				center += vecToSheep;
			}
		}
	}
	return center * avoidanceFactor;

}

CVec2 CHerdManager::MatchVelocity(CSheepActor* sheep)
{
	CVec2 velocity{ 0.0f,0.0f };
	//int count{ 0 };

	//for (auto otherSheep : m_herd) {
	//	if (otherSheep != sheep) {
	//		velocity += otherSheep->m_velocity;
	//		count++;
	//	}
	//}

	//velocity /= count;
	//CVec2 sheepNewVel = (velocity - sheep->m_velocity) * cohesionFactor;

	//CVec2 normNewVel = CVec2::Normalised(sheepNewVel);
	//CVec2 normVel = CVec2::Normalised(sheep->m_orientation);

	CVec2 spriteDefaultDir{ 0.0f, 1.0f };
	float cross = CVec2::cross(sheep->m_orientation, spriteDefaultDir);
	float dot = CVec2::dot(sheep->m_orientation, spriteDefaultDir);
	//float x = dot / sheepNewVel.Length() * sheep->m_velocity.Length();

	auto angle = acos(dot)/*-Math::pi/2*/;
	if (cross > 0) {
		angle = -angle;
	}

	//sheepNewVel.Rotate(angle);
	sheep->GetSprite()->GetSprite()->SetAngle(angle);

	return velocity;

}

void CHerdManager::LimitVelocity(CSheepActor* sheep)
{
	float maxVel{ 0.2 };

	if (sheep->m_velocity.Length() > maxVel) {
		sheep->m_velocity = (sheep->m_velocity / sheep->m_velocity.Length()) * maxVel;
	}
}

void CHerdManager::RandomiseVelocity(CSheepActor* sheep)
{
	float x = (0 + rand() % 1);
	float y = (0 + rand() % 1);
	CVec2 dir{ x, y };
	//dir.Normalised();
	sheep->m_velocity += dir * 1.11;
}

CVec2 CHerdManager::BindPosition(CSheepActor* sheep, float deltaTime)
{
	int xMin = 50;
	int yMin = 50;
	int xMax = APP_VIRTUAL_WIDTH-50;
	int yMax = APP_VIRTUAL_HEIGHT-50;

	CVec2 vec{ 0.0f,0.0f };
	CVec2 predictedPos = CPhysicsManager::Get().GetPredictedPosition(sheep, sheep->m_velocity*2.5f, deltaTime);






	if (predictedPos.m_x <= xMin) {
		sheep->m_velocity.m_x = 0;
		float cross = CVec2::cross(sheep->m_orientation, CVec2{ 1.0f,0.0f });
		float dot = CVec2::dot(sheep->m_orientation, CVec2{ 1.0f,0.0f });

		auto angle = acos(dot);
		if (cross > 0) {
			angle = -angle;
		}
		sheep->m_velocity.Rotate(angle);
	}
	else if (predictedPos.m_x >= xMax) {
		//vec.m_x = 0;
		sheep->m_velocity.m_x = 0;

	}
	if (predictedPos.m_y <= yMin) {
		//vec.m_y = 0;
		sheep->m_velocity.m_y = 0;

	}
	else if (predictedPos.m_y >= yMax) {
		//vec.m_y = 0;
		sheep->m_velocity.m_y = 0;

	}

	return vec;
}

void CHerdManager::SetIsGrazing(CSheepActor* sheep)
{
	if (sheep->m_isGrazing) {
		sheep->m_currentGrazeTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> totalGrazeTime = sheep->m_currentGrazeTime - sheep->m_startGrazeTime;
		if (totalGrazeTime > sheep->m_grazeCoolDown) {
			sheep->m_isGrazing = false;
			sheep->m_startGrazeSubmitTime = std::chrono::steady_clock::now();

			//DEBUG
			sheep->GetSprite()->GetSprite()->SetFrame(0);
		}
	}
	else {
		if (sheep->CanGraze()) {
			int isGrazing = rand() % 1000;
			if (isGrazing < grazingChance) {
				sheep->m_startGrazeTime = std::chrono::steady_clock::now();
				sheep->m_isGrazing = true;
				sheep->m_velocity *= 0.0f;

				//DEBUG
				sheep->GetSprite()->GetSprite()->SetFrame(1);
			}
			else {
				sheep->m_isGrazing = false;
			}
		}
	}
}

void CHerdManager::DetectEnclosure(CSheepActor* sheep)
{
	int type = CTileMap::Get().GetTileType(sheep->GetPosition());
	
	if (type == 0) {
		sheep->m_isActive = false;
		sheep->GetSprite()->GetSprite()->SetFrame(1);
	}
}

void CHerdManager::ResetSheep()
{
	for (auto sheep : m_herd) {
		sheep->m_isActive = true;
		sheep->GetSprite()->GetSprite()->SetFrame(0);
	}
}

const int& CHerdManager::GetDeadSheepCount()
{
	int deadSheepCount{ 0 };
	for (auto sheep : m_herd) {
		if (!sheep->m_isActive) {
			deadSheepCount++;
		}
	}
	return deadSheepCount;
}

