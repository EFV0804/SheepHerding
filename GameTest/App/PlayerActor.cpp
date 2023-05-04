#include "stdafx.h"
#include "PlayerActor.h"

CPlayerActor::CPlayerActor(CVec2 position, int order): CActor(position, order)
{
	m_sprite = new CSpriteComponent(this,10, ".\\TestData\\dog.bmp", 8, 1);
	m_sprite->GetSprite()->SetFrame(0);

	float speed = 1.0f / 15.0f;
	m_sprite->GetSprite()->CreateAnimation(ANIM_BARK,speed, { 0,1,2,3,4,5,6,7 });
	
	//TODO MAKE SPRITE SIZE STATIC CONST
	m_bb = new CBoundingBoxComponent(this, 10, CVec2(128.0f,128.0f));
	m_inputs = new CInputComponent(this, 10);
	m_inputs->SetSpeed(8.0f);
	m_force = m_baseForce;
}

void CPlayerActor::DrawBoundingBox()
{
	m_bb->Render();
}

void CPlayerActor::UpdateActor(float deltaTime)
{
	if (m_hasBarked) {
		m_currentBarkTime = std::chrono::steady_clock::now();
		int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(m_currentBarkTime - m_startBarkTime).count();

		if (elapsedTime > m_barkCoolDown) {
			m_sprite->GetSprite()->SetAnimation(-1);
			m_sprite->GetSprite()->SetFrame(0);
			m_force = m_baseForce;
		}
	}
	m_inputs->Update(deltaTime);
}

void CPlayerActor::ButtonAction(int button)
{
	if (button == XINPUT_GAMEPAD_A && m_force == m_baseForce) {
		m_startBarkTime = std::chrono::steady_clock::now();
		m_sprite->GetSprite()->SetAnimation(0);
		m_force += m_forceBoost;
		m_hasBarked = true;
	}
}
