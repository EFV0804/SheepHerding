#include "stdafx.h"
#include "PlayerActor.h"
//#include "app.h"
//#include <iostream>

CPlayerActor::CPlayerActor(CVec2 position): CActor(position)
{
	m_sprite = new CSpriteComponent(this, 10, ".\\TestData\\dog.bmp", 1, 1);
	m_bb = new CBoundingBoxComponent(this, 10, CVec2(128.0f,128.0f));
	m_inputs = new CInputComponent(this, 10);
	m_inputs->SetSpeed(8.0f);
	m_force = m_baseForce;
}

void CPlayerActor::Render()
{
	App::Print(50.0f, 50.0f, "TEST");
	//DrawBoundingBox();
	for (auto component : m_components) {
		if (component->IsRenderable()) {
			component->Render();
		}
	}
}

void CPlayerActor::DrawBoundingBox()
{
	m_bb->Render();
}

void CPlayerActor::UpdateActor(float deltaTime)
{
	if (hasBarked) {
		m_currentBarkTime = std::chrono::steady_clock::now();
		int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(m_currentBarkTime - m_startBarkTime).count();
		if (elapsedTime > barkCoolDown) {
			m_force = m_baseForce;
		}

	}
	m_inputs->Update(deltaTime);
}

void CPlayerActor::ButtonAction(int button)
{
	if (button == XINPUT_GAMEPAD_A && m_force == m_baseForce) {
		m_startBarkTime = std::chrono::steady_clock::now();
		m_force += m_forceBoost;
		hasBarked = true;


	}
}
