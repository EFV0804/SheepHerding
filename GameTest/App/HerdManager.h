#pragma once
#include "Vec2.h"
#include "SheepActor.h"
#include "PlayerActor.h"


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
	CVec2 MatchVelocity(CSheepActor* sheep);
	void LimitVelocity(CSheepActor* sheep);
	void RandomiseVelocity(CSheepActor* sheep);
	CVec2 BindPosition(CSheepActor* sheep, float deltaTime);
	void SetIsGrazing(CSheepActor* sheep);
	void DetectEnclosure(CSheepActor* sheep);
	void ResetSheep();
	const int& GetSheepCount() { return m_sheepCount; }
	const int& GetDeadSheepCount();

	std::vector<CSheepActor*>& GetHerd() { return m_herd; }
	void SetDog(CPlayerActor* dog) { m_dog = dog; }

private:
	CVec2 m_position{200.0f, 100.0f };
	float m_radius{ 100.0f };
	std::vector<CSheepActor*> m_herd;
	CPlayerActor* m_dog = nullptr;
	int m_sheepCount{ 0 };
};

