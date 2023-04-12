#pragma once
#include "Vec2.h"
#include "SheepActor.h"

class CHerdManager
{
public:
	CHerdManager();
	CHerdManager(const CHerdManager&) = delete;
	CHerdManager& operator=(const CHerdManager&) = delete;

	void MakeHerd(int numSheep);
	void Update(float deltaTime);
	void ComputePosition();
	std::vector<CSheepActor*>& GetHerd() { return m_herd; }

private:
	std::vector<CSheepActor*> m_herd;
	CVec2 m_position;
	float m_radius;
};

