#include "stdafx.h"
#include "HerdManager.h"
#include <cstdlib>
#include <time.h>

CHerdManager::CHerdManager() : m_position{ 100.0,100.0 }, m_radius{ 75.0 }
{

}

void CHerdManager::MakeHerd(int numSheep)
{
	srand(time(0));

	for (int i = 0; i < numSheep; i++) {

		float x = 1 + rand() % 500;
		float y = 1 + rand() % 200;
		CVec2 randPos{x,y};

		m_herd.push_back(new CSheepActor(randPos));
	}
}

void CHerdManager::Update(float deltaTime)
{
	ComputePosition();

	for (auto sheep : m_herd) {
		CVec2 vecToHerd = m_position - sheep->GetPosition();
		float distanceToHerd = vecToHerd.Length();
		vecToHerd.Normalised();
		if (distanceToHerd > m_radius) {
			sheep->SetPosition(sheep->GetPosition() + vecToHerd * (0.2 * deltaTime));
		}
	}
}

void CHerdManager::ComputePosition()
{
	float x{ 0.0f };
	float y{ 0.0f };

	for (auto sheep : m_herd) {
		x += sheep->GetPosition().m_x;
		y += sheep->GetPosition().m_y;
	}
	m_position = CVec2(x / m_herd.size() , y / m_herd.size());
}
