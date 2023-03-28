#include "stdafx.h"
#include <windows.h> 
#include "Game.h"
#include "app.h"
#include "TileMap.h"

void CGame::Init()
{
	CTileMap tileMap{};
	tileMap.Load();
	CPlayerActor* player = new CPlayerActor(CVec2(600.0f, 600.0f));
	CSheepActor* sheep = new CSheepActor(CVec2(100.0f, 100.0f));
}

void CGame::Update(float deltaTime)
{
	//if (App::AABBIntersects(player->GetBoundingBox(), sheep->GetBoundingBox())) {
	//	App::PlaySound(".\\TestData\\Test.wav");
	//}
	//else {

	//}

	for (auto actor : m_actors) {
		actor->Update(deltaTime);
	}

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
