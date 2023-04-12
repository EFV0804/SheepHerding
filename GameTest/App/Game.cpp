#include "stdafx.h"
#include <windows.h> 
#include "Game.h"
#include "app.h"
#include "TileMap.h"


void CGame::Init()
{
	CTileMap tileMap{};
	tileMap.Load();
	CPlayerActor* player = new CPlayerActor(CVec2(500.0f, 400.0f));
	player->SetForce(200.0f);

	m_physics = CPhysicsManager();
	m_physics.AddBody(player->GetBoundingBox());

	m_herd.MakeHerd(5);
	for (auto sheep : m_herd.GetHerd()) {
		m_physics.AddBody(sheep->GetBoundingBox());
	}
}

void CGame::PreUpdate(float deltaTime)
{
	m_physics.Update(deltaTime);
}

void CGame::Update(float deltaTime)
{
	for (auto actor : m_actors) {
		actor->Update(deltaTime);
	}
	m_herd.Update(deltaTime);

}

void CGame::Render()
{
	for (auto actor : m_actors) {
		actor->Render();
	}
}

void CGame::Shutdown()
{
	while (!m_actors.empty()) {
		delete m_actors.back();
	}
}

void CGame::AddActor(CActor* actor)
{
	m_actors.emplace_back(actor);
}

void CGame::RemoveActor(CActor* actor)
{
	auto iter = std::find(begin(m_actors), end(m_actors), actor);
	if (iter != end(m_actors)) {
		std::iter_swap(iter, end(m_actors) - 1);
		m_actors.pop_back();
	}
}
