#include "stdafx.h"
#include "SheepActor.h"
#include "SpriteComponent.h"

CSheepActor::CSheepActor(CVec2 position) : CActor(position)
{
	m_sprite = new CSpriteComponent(this, 11, ".\\TestData\\sheep.bmp", 1, 1);
	m_bb = new CBoundingBoxComponent(this, 10, CVec2(128.0f, 128.0f));
	SetForce(50.0f);
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
