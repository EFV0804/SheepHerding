#include "stdafx.h"
#include "Actor.h"
#include "Game.h"

CActor::CActor(CVec2 position, int order) : 
	m_position{ position },
	m_refPosition{ position },
	m_updateOrder{order}
{
	CGame::Get().AddActor(this);
}
CActor::~CActor() {

	CGame::Get().RemoveActor(this);

	while (!m_components.empty()) {

		delete m_components.back();
	}
}

void CActor::Update(float deltaTime)
{
	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
}

void CActor::UpdateActor(float deltaTime)
{
}

void CActor::UpdateComponents(float deltaTime)
{
	for (auto component : m_components) {
		component->Update(deltaTime);
	}
}

void CActor::Render()
{
	for (auto component : m_components) {
		if (component->IsRenderable()) {
			component->Render();
		}
	}

}

void CActor::SetPosition(CVec2& position)
{
	m_position = position;
	CVec2 diff = position - m_refPosition;
	if (diff.Length() > 10.0f) {
		m_orientation = CVec2::Normalised(diff);
		m_refPosition = position;
	}
}

void CActor::AddComponent(CComponent* component)
{
	int order = component->GetOrder();
	auto iter = begin(m_components);
	for (; iter != end(m_components); ++iter) {
		if (order < (*iter)->GetOrder()) {
			break;
		}
	}
	m_components.insert(iter,component);
}

void CActor::RemoveComponent(CComponent* component)
{
	auto iter = std::find(begin(m_components), end(m_components), component);
	if (iter != end(m_components)) {
		m_components.erase(iter);
	}
}

void CActor::ButtonAction(int button)
{

}


