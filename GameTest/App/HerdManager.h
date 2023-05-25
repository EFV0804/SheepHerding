#pragma once
#include "Vec2.h"
#include "SheepActor.h"
#include "PlayerActor.h"
#include "TileMap.h"


class CHerdManager
{
public:
	CHerdManager();
	CHerdManager(const CHerdManager&) = delete;
	CHerdManager& operator=(const CHerdManager&) = delete;

	void MakeHerd(int sheepCount);
	void ComputeDog(float deltaTime);
	void ComputeSheep(float deltaTime);
	void Update(float deltaTime);

	void ComputePosition();
	CVec2 MoveToCenter(CSheepActor * sheep);
	CVec2 AvoidOthers(CSheepActor* sheep);
	void MatchVelocity(CSheepActor* sheep);
	CVec2 Flee(CSheepActor* sheep,const CVec2& target);
	void LimitVelocity(CSheepActor* sheep);
	void RandomiseVelocity(CSheepActor* sheep);
	void SetIsGrazing(CSheepActor* sheep);
	void DetectEnclosure(CSheepActor* sheep);
	void ResetSheep();
	const int& GetSheepCount() { return m_sheepCount; }
	const int& GetDeadSheepCount();
	void isFacingWall(CSheepActor* sheep);

	std::vector<CSheepActor*>& GetHerd() { return m_herd; }
	void SetDog(CPlayerActor* dog) { m_dog = dog; }

private:
	CVec2 m_position{200.0f, 100.0f };
	float m_radius{ 100.0f };
	std::vector<CSheepActor*> m_herd;
	int m_activeSheep{0};
	int m_grazingSheep{ 0 };
	CPlayerActor* m_dog = nullptr;
	int m_sheepCount{ 0 };
	int m_sheepForce{ 20 };
	std::vector<CTileActor*> m_grassTiles;
};

