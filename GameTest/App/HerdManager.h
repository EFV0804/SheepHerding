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

	CVec2 MoveToCenter(CSheepActor * sheep);
	CVec2 AvoidOthers(CSheepActor* sheep);
	CVec2 MatchVelocity(CSheepActor* sheep);
	void LimitVelocity(CSheepActor* sheep);
	void RandomiseVelocity(CSheepActor* sheep);
	CVec2 BindPosition(CSheepActor* sheep);
	void SetIsGrazing(CSheepActor* sheep);

	std::vector<CSheepActor*>& GetHerd() { return m_herd; }
	void SetDog(CPlayerActor* dog) { m_dog = dog; }

private:
	std::vector<CSheepActor*> m_herd;
	CPlayerActor* m_dog = nullptr;
};

