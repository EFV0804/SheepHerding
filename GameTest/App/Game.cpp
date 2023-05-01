#include "stdafx.h"
#include <windows.h> 
#include "Game.h"
#include "app.h"
#include "TileMap.h"

#include <format>



void CGame::Init()
{


	CTileMap::Get().Load();


	m_herd.MakeHerd(15);
	for (auto sheep : m_herd.GetHerd()) {
		CPhysicsManager::Get().AddBody(sheep->GetBoundingBox());
	}
	CPlayerActor* player = new CPlayerActor(CVec2(500.0f, 400.0f));
	CPhysicsManager::Get().AddBody(player->GetBoundingBox());
	m_herd.SetDog(player);

	timer.Start(true, m_gameLoopTime);
	//timer.Start();
}

void CGame::PreUpdate(float deltaTime)
{
	m_herd.Update(deltaTime);
	CPhysicsManager::Get().Update(deltaTime);
}

void CGame::Update(float deltaTime)
{
	ProcessInputs();


	if (!isPaused) {

		for (auto actor : m_actors) {
			actor->Update(deltaTime);
		}

		bool isWin = m_herd.GetDeadSheepCount() == m_herd.GetSheepCount();
		bool isOutOfTime = timer.GetElapsedTime() > std::chrono::duration<double>(m_gameLoopTime);

		if (isWin || isOutOfTime) {
			timer.Reset();
			timer.Start(true, m_gameLoopTime);
			//timer.Start();
			CTileMap::Get().Load();
			m_herd.ResetSheep();
		}
	}

}

void CGame::Render()
{


	for (auto actor : m_actors) {
		if (actor->GetForce() == 200) {
			actor->Render();
		}
		else {
			actor->Render();
		}

	}
	char buf1[50];
	char buf[50];
	int activeSheep = m_herd.GetSheepCount();
	int deadSheep{ 0 };
	int timeRemaining{ 0 };

	if(!isPaused) {
		deadSheep= m_herd.GetDeadSheepCount();
		timeRemaining = (int)timer.GetElapsedTime().count();
	}
	sprintf(buf1, "Time Remaining: %d", timeRemaining);

	sprintf(buf, " Gathered Sheep: %d/%d", deadSheep, activeSheep);

	App::Print(20, 1000, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	App::Print(20, 950, buf1, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
}

void CGame::Shutdown()
{
	while (!m_actors.empty()) {
		delete m_actors.back();
	}
}

void CGame::ProcessInputs()
{
	if (m_controller.CheckButton(XINPUT_GAMEPAD_START)) {
		if (isPaused) {
			isPaused = false;
			timer.Start();
		}
		else {
			isPaused = true;
			timer.Stop();

		}
	}
}

void CGame::AddActor(CActor* actor)
{
	int order = actor->GetOrder();
	auto iter = begin(m_actors);
	for (; iter != end(m_actors); ++iter) {
		if (order < (*iter)->GetOrder()) {
			break;
		}
	}
	m_actors.insert(iter, actor);
}

void CGame::RemoveActor(CActor* actor)
{
	auto iter = std::find(begin(m_actors), end(m_actors), actor);
	if (iter != end(m_actors)) {
		std::iter_swap(iter, end(m_actors) - 1);
		m_actors.pop_back();
	}
}

