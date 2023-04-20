#include "stdafx.h"
#include "HerdManager.h"
#include <cstdlib>
#include <time.h>
#include "PhysicsManager.h"

CHerdManager::CHerdManager()
{

}

void CHerdManager::MakeHerd(int sheepCount)
{
	srand(time(0));

	for (int i = 0; i < sheepCount; i++) {

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
	for (auto sheep : m_herd) {

		SetIsGrazing(sheep);

		if (!sheep->m_isGrazing) {
			CVec2 centerVec = MoveToCenter(sheep);
			CVec2 avoidanceVec = AvoidOthers(sheep);
			CVec2 velocityVec = MatchVelocity(sheep);
			CVec2 boundVelocity = BindPosition(sheep);

			sheep->m_velocity += centerVec + avoidanceVec + velocityVec + boundVelocity;
		}

		LimitVelocity(sheep);
		RandomiseVelocity(sheep);

		CPhysicsManager::Get().UpdateActorPosition(sheep->GetBoundingBox(), sheep->m_velocity, deltaTime);
	}
}

void CHerdManager::Update(float deltaTime)
{
	ComputeDog(deltaTime);
	ComputeSheep(deltaTime);
}

CVec2 CHerdManager::MoveToCenter(CSheepActor* sheep)
{

	CVec2 center{ 0.0f, 0.0f };
	int count{ 0 };
	int centeringFactor{100000};

	for (auto otherSheep : m_herd) {
		if (otherSheep != sheep) {
			center += otherSheep->GetPosition();
			count++;
		}
	}

	center /= count - 1;
	CVec2 vecToHerd = (center - sheep->GetPosition()) / centeringFactor;

	return vecToHerd;

}

CVec2 CHerdManager::AvoidOthers(CSheepActor* sheep)
{
	CVec2 center{ 0.0f,0.0f };
	float avoidanceFactor{0.25};

	for (auto otherSheep : m_herd) {
		if (otherSheep != sheep) {
			CVec2 vecToSheep = otherSheep->GetPosition() - sheep->GetPosition();
			float distanceBetweenSheep = vecToSheep.Length();
			
			if (distanceBetweenSheep < avoidanceFactor) {
				center -= (otherSheep->GetPosition() - sheep->GetPosition());
			}
		}
	}

	return center;

}

CVec2 CHerdManager::MatchVelocity(CSheepActor* sheep)
{
	CVec2 velocity{ 0.0f,0.0f };
	int count{ 0 };
	float cohesionFactor{ 10000 };

	for (auto otherSheep : m_herd) {
		if (otherSheep != sheep) {
			velocity += otherSheep->m_velocity;
			count++;
		}
	}

	velocity /= (count - 1);
	CVec2 sheepNewVel = (velocity - sheep->m_velocity) / cohesionFactor;

	return sheepNewVel;
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
	sheep->m_velocity += dir *0.01;
}

CVec2 CHerdManager::BindPosition(CSheepActor* sheep)
{
	int xMin = 100;
	int yMin = 100;
	int xMax = APP_INIT_WINDOW_WIDTH-100;
	int yMax = APP_INIT_WINDOW_HEIGHT-100;

	CVec2 vec{ 0.0f,0.0f };

	if (sheep->GetPosition().m_x < xMin) {
		vec.m_x = 10;
	}
	else if (sheep->GetPosition().m_x > xMax) {
		vec.m_x = -10;
	}
	if (sheep->GetPosition().m_y < yMin) {
		vec.m_y = 10;
	}
	else if (sheep->GetPosition().m_y > yMax) {
		vec.m_y = -10;
	}

	return vec;
}

void CHerdManager::SetIsGrazing(CSheepActor* sheep)
{
	if (sheep->m_isGrazing) {
		sheep->m_currentGrazeTime = std::chrono::steady_clock::now();
		int totalGrazeTime = std::chrono::duration_cast<std::chrono::seconds>(sheep->m_currentGrazeTime - sheep->m_startGrazeTime).count();
		if (totalGrazeTime > sheep->m_grazeCoolDown) {
			sheep->m_isGrazing = false;
		}
	}
	else {
		int isGrazing = rand() % 200;
		if (isGrazing < 2) {
			sheep->m_startGrazeTime = std::chrono::steady_clock::now();
			sheep->m_isGrazing = true;
			sheep->m_velocity *= 0;
		}
		else {
			sheep->m_isGrazing = false;
		}
	}
}

