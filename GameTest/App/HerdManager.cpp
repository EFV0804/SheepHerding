#include "stdafx.h"
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include "HerdManager.h"
#include "PhysicsManager.h"
#include "TileMap.h"
#include "Math.h"

constexpr float centeringFactor{ 0.01f };
constexpr float avoidanceFactor{0.5f };
constexpr float cohesionFactor{ 0.1f };
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
		x = rand() % 1;
		y = rand() % 1;
		m_herd.back()->m_velocity = CVec2{x, y};
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
			CPhysicsManager::Get().UpdateActorPosition(sheep, force*0.002f, deltaTime);
		}
	}
}

void CHerdManager::ComputeSheep(float deltaTime)
{

	for (auto sheep : m_herd) {
		//DetectEnclosure(sheep);

		if (sheep->m_isActive) {

			//SetIsGrazing(sheep);


			if (!sheep->m_isGrazing) {
				CVec2 centerVec = MoveToCenter(sheep);
				sheep->m_velocity += centerVec;
				CVec2 avoidanceVec = AvoidOthers(sheep);
				sheep->m_velocity += avoidanceVec;
				CVec2 velocityVec = MatchVelocity(sheep);
				sheep->m_velocity += velocityVec;

				CVec2 fleeVec = Flee(sheep, m_dog);
				sheep->m_velocity += fleeVec;

			}

			LimitVelocity(sheep);
			//RandomiseVelocity(sheep);

			CPhysicsManager::Get().UpdateActorPosition(sheep, sheep->m_velocity, deltaTime);


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
		if (sheep->m_isActive) {
			x += sheep->GetPosition().m_x;
			y += sheep->GetPosition().m_y;
		}
	}
	m_position = CVec2(x / m_herd.size(), y / m_herd.size());
}

CVec2 CHerdManager::MoveToCenter(CSheepActor* sheep)
{
	ComputePosition();

	CVec2 vecToHerd = m_position - sheep->GetPosition();
	float distanceToHerd = vecToHerd.Length();
	CVec2 dirToHerd = CVec2::Normalised(vecToHerd);
	bool isTowardsHerd = true;
	float dot = CVec2::dot(CVec2::Normalised(sheep->m_velocity), dirToHerd);

	if(dot < 1 && distanceToHerd < m_radius) {
		isTowardsHerd = false;
	}

	if (isTowardsHerd) {
		if (distanceToHerd > m_radius) {
			CVec2 steering = vecToHerd - sheep->m_velocity;
			return steering ;

		}
		else {
			sheep->m_velocity *= 0.5;
			return CVec2(0.0f, 0.0f);
		}
	}
	else {

		sheep->m_velocity *= 0.5;
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
		if (otherSheep != sheep && sheep->m_isActive) {
			CVec2 vecToSheep = sheep->GetPosition() - otherSheep->GetPosition();
			float distanceBetweenSheep = vecToSheep.Length();

			if (distanceBetweenSheep < m_sheepForce) {
				center += vecToSheep;
			}
		}
	}

	ComputePosition();
	CVec2 vecToHerd = m_position - sheep->GetPosition();
	float distanceToHerd = vecToHerd.Length();

	if (distanceToHerd <= m_radius) {
		float reductionFactor = avoidanceFactor * 0.005;
		return center*reductionFactor;
	}
	else {
		return center * avoidanceFactor;
	}


}

CVec2 CHerdManager::MatchVelocity(CSheepActor* sheep)
{
	CVec2 velocity{ 0.0f,0.0f };
	//int count{ 0 };

	//for (auto otherSheep : m_herd) {
	//	if (otherSheep != sheep && sheep->m_isActive) {
	//		velocity += otherSheep->m_velocity;
	//		count++;
	//	}
	//}

	//velocity /= count;
	//CVec2 sheepNewVel = (velocity - sheep->m_velocity) * cohesionFactor;


	//ORIENTATION
	CVec2 spriteDefaultDir{ 0.0f, 1.0f };
	float cross = CVec2::cross(sheep->m_orientation, spriteDefaultDir);
	float dot = CVec2::dot(sheep->m_orientation, spriteDefaultDir);

	auto angle = acos(dot);
	if (cross > 0) {
		angle = -angle;
	}
	sheep->GetSprite()->GetSprite()->SetAngle(angle);

	//END

	return velocity;

}

CVec2 CHerdManager::Flee(CSheepActor* sheep, CActor* target)
{
	if ((target->GetPosition() - sheep->GetPosition()).Length() < 200.0f) {
		CVec2 targetToSheep = sheep->GetPosition() - target->GetPosition();
		CVec2 desiredVel = CVec2::Normalised(targetToSheep)*0.2;
		CVec2 steeringVec = desiredVel - sheep->m_velocity;
		steeringVec.Normalised();
		steeringVec *= 0.003;
		return steeringVec;
	}
	else {
		return CVec2{0.0f,0.0f};
	}
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
				//TODO MOVE AWAY FROM HERD
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
	//RESET POSITION
	// int type{0};
	// while do(type == 0){
	// 	type = CTileMap::Get().GetTileType(sheep->GetPosition());
	// 
		//CVec2 newPos{ rand % 900, rand % 600 };
	// }
	// 
	// sheep.SetPosition(newPos);
	
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

