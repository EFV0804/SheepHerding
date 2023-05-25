#include "stdafx.h"
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <random>
#include "HerdManager.h"
#include "PhysicsManager.h"
#include "TileMap.h"
#include "Math.h"

constexpr float centeringFactor{ 0.01f };
constexpr float avoidanceFactor{0.5f };
constexpr float cohesionFactor{ 0.1f };
constexpr float grazingChance{ 0.5f };

CHerdManager::CHerdManager()
{

}

void CHerdManager::MakeHerd(int sheepCount)
{
	
	m_sheepCount = sheepCount;
	m_activeSheep = m_sheepCount;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distrX(100, 900); // define the range
	std::uniform_int_distribution<> distrY(100, 600); // define the range

	////srand(time(0));

	for (int i = 0; i < m_sheepCount; i++) {

		float x = distrX(gen);
		float y = distrY(gen);
		CVec2 randPos{x,y};

		m_herd.push_back(new CSheepActor(randPos));
		std::uniform_int_distribution<> distrVel(0.2, 1); // define the range
		x = distrVel(gen);
		y = distrVel(gen);
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
		DetectEnclosure(sheep);

		if (sheep->m_isActive) {

			


			if (!sheep->m_isGrazing) {
				CVec2 centerVec = MoveToCenter(sheep);
				sheep->m_velocity += centerVec;
				CVec2 avoidanceVec = AvoidOthers(sheep);
				sheep->m_velocity += avoidanceVec;
				CVec2 fleeVec = Flee(sheep, m_dog);
				sheep->m_velocity += fleeVec;
				//MatchVelocity(sheep);
			}

			SetIsGrazing(sheep);
			MatchVelocity(sheep);
			RandomiseVelocity(sheep);

			LimitVelocity(sheep);


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
	int sheepCount = 0;
	for (auto sheep : m_herd) {
		if (sheep->m_isActive) {
			x += sheep->GetPosition().m_x;
			y += sheep->GetPosition().m_y;
			sheepCount++;
		}
	}

	m_position = CVec2(x / sheepCount, y / sheepCount);
}

CVec2 CHerdManager::MoveToCenter(CSheepActor* sheep)
{
	ComputePosition();

	if (sheep->m_isActive) {
		CVec2 vecToHerd = m_position - sheep->GetPosition();
		float distanceToHerd = vecToHerd.Length();
		CVec2 dirToHerd = CVec2::Normalised(vecToHerd);
		bool isTowardsHerd = true;
		float dot = CVec2::dot(CVec2::Normalised(sheep->m_velocity), dirToHerd);

		if (dot < 1 && distanceToHerd < m_radius) {
			isTowardsHerd = false;
		}

		if (isTowardsHerd) {
			if (distanceToHerd > m_radius) {
				CVec2 steering = vecToHerd - sheep->m_velocity;
				return steering;

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
	}
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

void CHerdManager::MatchVelocity(CSheepActor* sheep)
{
	/*CVec2 velocity{ 0.0f,0.0f };*/
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
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distrVel(0, 0.7); // define the range

	float x = distrVel(gen);
	float y = distrVel(gen);
	CVec2 dir{ x, y };
	//dir.Normalised();
	sheep->m_velocity += dir /** 1.11*/;
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

			std::random_device rd; // obtain a random number from hardware
			std::mt19937 gen(rd()); // seed the generator
			std::uniform_int_distribution<> distrGraze(0, 1500); // define the range

			int isGrazing = distrGraze(gen);

			if (isGrazing < grazingChance) {
				sheep->m_startGrazeTime = std::chrono::steady_clock::now();
				sheep->m_isGrazing = true;

				CVec2 vecToHerd = m_position - sheep->GetPosition();
				vecToHerd.m_x = vecToHerd.m_x * -1;
				vecToHerd.m_y = vecToHerd.m_y * -1;
				vecToHerd.Normalised();

				//TODO MOVE AWAY FROM HERD
				sheep->m_velocity += vecToHerd*0.15;
				
				//DEBUG
				//sheep->GetSprite()->GetSprite()->SetFrame(1);
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
		m_activeSheep -= 1;
		sheep->GetSprite()->GetSprite()->SetFrame(1);
	}
}

void CHerdManager::ResetSheep()
{
	for (auto sheep : m_herd) {
		sheep->m_isActive = true;
		m_activeSheep = m_sheepCount;
		sheep->GetSprite()->GetSprite()->SetFrame(0);

		//int type{ 0 };
		//CVec2 newPos{0.0f,0.0f};

		//do {
		//	type = CTileMap::Get().GetTileType(sheep->GetPosition());
		//	newPos = { float(1 + rand() % 900), float(1+rand() % 600) };
		//} while (type == 0);

		//sheep->SetPosition(newPos);
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

