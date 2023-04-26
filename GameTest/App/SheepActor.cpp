#include "stdafx.h"
#include "SheepActor.h"
#include "SpriteComponent.h"

CSheepActor::CSheepActor(CVec2 position, int order) : CActor(position, order)
{
	m_sprite = new CSpriteComponent(this, 11, ".\\TestData\\sheep.bmp", 2, 1);
	m_bb = new CBoundingBoxComponent(this, 10, CVec2(128.0f, 128.0f));
	SetForce(50.0f);
	m_startGrazeSubmitTime = std::chrono::steady_clock::now();
	m_grazeWait = std::chrono::duration<double>{ rand() % 5 };
}

void CSheepActor::Render()
{
	for (auto component : m_components) {
		if (component->IsRenderable()) {
			component->Render();
		}
	}
}

void CSheepActor::UpdateActor()
{

}

void CSheepActor::DrawBoundingBox()
{
	//m_bb->Render();
}

bool CSheepActor::CanGraze()
{
	m_currentGrazeSubmitTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> totalWaitTime = m_currentGrazeSubmitTime - m_startGrazeSubmitTime;
	if (totalWaitTime > m_grazeWait) {
		return true;
	}
	return false;
}
