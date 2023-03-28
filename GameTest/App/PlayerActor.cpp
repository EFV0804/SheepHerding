#include "stdafx.h"
#include "PlayerActor.h"
#include "app.h"

CPlayerActor::CPlayerActor(CVec2 position): CActor(position)
{
	m_sprite = new CSpriteComponent(this, 10, ".\\TestData\\dog.bmp", 1, 1);
	m_bb = new CBoundingBoxComponent(this, 10, CVec2(200.0f,200.0f));
	m_inputs = new CInputComponent(this, 10);
	m_inputs->SetSpeed(10.0f);
}

void CPlayerActor::Render()
{
	for (auto component : m_components) {
		if (component->IsRenderable()) {
			component->Render();
		}
	}

	m_bb->Render();
}

void CPlayerActor::DrawBoundingBox()
{
	m_bb->Render();
}

void CPlayerActor::UpdateActor(float deltaTime)
{
	m_inputs->Update();
}
