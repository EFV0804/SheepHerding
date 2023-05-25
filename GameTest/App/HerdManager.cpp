#include "stdafx.h"
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <random>
#include "HerdManager.h"
#include "PhysicsManager.h"
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

	srand(time(0));

	//for (auto tile : CTileMap::Get().GetTiles()) {
	//	if (CTileMap::Get().GetTileType(tile->GetPosition()) == 1
	//		&& tile->GetPosition().m_y < 750
	//		&& tile->GetPosition().m_y > 100
	//		&& tile->GetPosition().m_x < 950
	//		&& tile->GetPosition().m_x > 100) {

	//		m_grassTiles.push_back(tile);
	//	}
	//}

	//std::vector<CTileActor*>* vec = CTileMap::Get().GetGrassTiles();
	int x = rand() % CTileMap::Get().GetGrassTiles().size();
	CVec2 pos = CTileMap::Get().GetGrassTiles().at(x)->GetPosition();
	int randomFactor = 0 + rand() % 15;
	pos.m_x += randomFactor;
	pos.m_y += randomFactor;

	for (int i = 0; i < m_sheepCount; i++) {

		//float x = distrX(gen);
		//float y = distrY(gen);
		//CVec2 randPos{x,y};

		m_herd.push_back(new CSheepActor(pos));
		std::uniform_int_distribution<> distrVel(0.2, 1); // define the range
		float x = distrVel(gen);
		float y = distrVel(gen);
		m_herd.back()->m_velocity = CVec2{x, y};
	}
}

void CHerdManager::ComputeDog(float deltaTime)
{
	for (auto sheep : m_herd) {
		if (sheep->m_isActive) {
			CVec2 distanceVec = m_dog->GetPosition() - sheep->GetPosition();
			float distance = distanceVec.Length();
			if (distance < m_dog->GetForce()) {
				CVec2 dir = sheep->GetPosition() - m_dog->GetPosition();
				dir.Normalised();
				float repelStrength = m_dog->GetForce() - distance;
				CVec2 force = repelStrength * dir;
				CPhysicsManager::Get().UpdateActorPosition(sheep, force * 0.002f, deltaTime);
			}
		}
		if (!sheep->m_isActive) {
			CVec2 distanceVec = m_dog->GetPosition() - sheep->GetPosition();
			float distance = distanceVec.Length();
			if (distance < m_dog->GetForce()) {
				CVec2 dir = sheep->GetPosition() - m_dog->GetPosition();
				dir.Normalised();
				float repelStrength = m_dog->GetForce() - distance;
				CVec2 force = (repelStrength*0.2)*dir;
				CPhysicsManager::Get().UpdateActorPosition(sheep, force * 0.002f, deltaTime);
			}
		}
	}
}

void CHerdManager::ComputeSheep(float deltaTime)
{
	for (auto sheep : m_herd) {


		if (sheep->m_isActive) {

			if (!sheep->m_isGrazing) {
				CVec2 centerVec = MoveToCenter(sheep);
				sheep->m_velocity += centerVec;
				CVec2 avoidanceVec = AvoidOthers(sheep);
				sheep->m_velocity += avoidanceVec;
				//CVec2 dogFleeVec = Flee(sheep, m_dog->GetPosition());
				//sheep->m_velocity += dogFleeVec;
				//CVec2 enclosurePos = CTileMap::Get().GetAverageEnclosurePos();
				//CVec2 enclosureFleeVec = Flee(sheep, enclosurePos);
				//sheep->m_velocity += enclosureFleeVec;
			}

			SetIsGrazing(sheep);
			MatchVelocity(sheep);
			RandomiseVelocity(sheep);
			LimitVelocity(sheep);

			CPhysicsManager::Get().UpdateActorPosition(sheep, sheep->m_velocity, deltaTime);

		}
		//else if (!sheep->m_isActive) {
		//	CVec2 average = CTileMap::Get().GetAverageEnclosurePos();
		//	CVec2 vecToEnclosure = average - sheep->GetPosition() ;
		//	vecToEnclosure.Normalised();
		//	float dist = vecToEnclosure.Length();
		//	dist *= 0.02;
		//	CVec2 vel = vecToEnclosure * dist ;
		//	sheep->m_velocity += vel;
		//}
		DetectEnclosure(sheep);
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

CVec2 CHerdManager::Flee(CSheepActor* sheep, const CVec2& target)
{
	CVec2 velocity{ 0.0f,0.0f };

	if ((target - sheep->GetPosition()).Length() < 200.0f) {
		CVec2 targetToSheep = sheep->GetPosition() - target;
		targetToSheep.Normalised();
		velocity = targetToSheep * 100.0f;
		return velocity;
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

		CVec2 vecToHerd = m_position - sheep->GetPosition();
		float distToHerd = vecToHerd.Length();

		sheep->m_currentGrazeTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> totalGrazeTime = sheep->m_currentGrazeTime - sheep->m_startGrazeTime;
		std::chrono::milliseconds minGrazeTime{ 2000 };

		if (distToHerd < m_radius+50 && totalGrazeTime > minGrazeTime) {
			sheep->m_isGrazing = false;
			m_grazingSheep--;
			//sheep->GetSprite()->GetSprite()->SetFrame(0);
		}
		else if (distToHerd >200.0f) {
			sheep->m_velocity = { 0.0f,0.0f };
		}
		
		//sheep->m_currentGrazeTime = std::chrono::steady_clock::now();
		//std::chrono::duration<double> totalGrazeTime = sheep->m_currentGrazeTime - sheep->m_startGrazeTime;
		//if (totalGrazeTime > sheep->m_grazeCoolDown) {
		//	sheep->m_isGrazing = false;
		//	sheep->m_startGrazeSubmitTime = std::chrono::steady_clock::now();

		//	//DEBUG
			//sheep->GetSprite()->GetSprite()->SetFrame(0);
		//}
	}
	else {
		if (sheep->CanGraze()) {

			std::random_device rd; // obtain a random number from hardware
			std::mt19937 gen(rd()); // seed the generator
			std::uniform_int_distribution<> distrGraze(0, 1000); // define the range

			int isGrazing = distrGraze(gen);

			if (isGrazing < grazingChance && m_grazingSheep < 4) {
				sheep->m_startGrazeTime = std::chrono::steady_clock::now();
				sheep->m_isGrazing = true;
				m_grazingSheep++;

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
	if (sheep->m_isActive) {
		if (type == 0) {
			sheep->m_isActive = false;
			m_activeSheep -= 1;
			sheep->GetSprite()->GetSprite()->SetFrame(1);
		}
	}
	//else {
	//	CVec2 average = CTileMap::Get().GetAverageEnclosurePos();
	//	CVec2 vecToEnclosure = average - sheep->GetPosition();
	//	vecToEnclosure.Normalised();
	//	float dist = vecToEnclosure.Length();
	//	sheep->m_velocity += vecToEnclosure * dist*0.002;
	//}


}

void CHerdManager::ResetSheep()
{
	//m_grassTiles = 
	//std::vector<CTileActor*>* vec = CTileMap::Get().GetGrassTiles();
	//for (auto tile : CTileMap::Get().GetTiles()) {
	//	if (CTileMap::Get().GetTileType(tile->GetPosition()) == 1
	//		&& tile->GetPosition().m_y < 800
	//		&& tile->GetPosition().m_y > 200
	//		&& tile->GetPosition().m_x < 800
	//		&& tile->GetPosition().m_x > 200) {

	//		m_grassTiles.push_back(tile);
	//	}
	//}
	int x = rand() % CTileMap::Get().GetGrassTiles().size();
	CVec2 pos = CTileMap::Get().GetGrassTiles().at(x)->GetPosition();
	//int randomFactor = 50 + rand() % 100;
	pos.m_x += 200;
	pos.m_y += 200;
	//int x = rand() % m_grassTiles.size();
	//CVec2 pos = m_grassTiles.at(x)->GetPosition();


	for (auto sheep : m_herd) {
		sheep->m_isActive = true;
		m_activeSheep = m_sheepCount;
		sheep->GetSprite()->GetSprite()->SetFrame(0);
		sheep->SetPosition(pos);

		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrVel(0.8, 1); // define the range

		float x = distrVel(gen);
		float y = distrVel(gen);
		m_herd.back()->m_velocity = CVec2{ x, y };
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

void CHerdManager::isFacingWall(CSheepActor* sheep)
{
	for (auto tile : CTileMap::Get().GetTiles()) {
		if (tile->GetType() == 2 || tile->GetType() == 0) {
			CVec2 vecToTile = tile->GetPosition() - sheep->GetPosition();
			vecToTile.Normalised();
			bool isFacingWell = CPhysicsManager::Get().RaycastIntersects(sheep->GetPosition(), vecToTile, sheep->GetBoundingBox());
			if (isFacingWell) {
				sheep->GetSprite()->GetSprite()->SetFrame(1);
			}
			//else {
			//	sheep->GetSprite()->GetSprite()->SetFrame(0);
			//}
		}

	}

}

