#include "stdafx.h"
#include <windows.h> 
#include "Game.h"
#include "app.h"
#include "TileMap.h"

#include <format>



void CGame::Init()
{
	CTileMap::Get().Load();
	m_herd.MakeHerd(20);
	CPlayerActor* player = new CPlayerActor(CVec2(500.0f, 400.0f));
	m_herd.SetDog(player);
	m_countdown.Start();
}

void CGame::PreUpdate(float deltaTime)
{
	if (!m_isPaused && !m_isWin || !m_isOutOfTime) {
		
		CPhysicsManager::Get().Update(deltaTime);
	}
}

void CGame::Update(float deltaTime)
{
	ProcessInputs();


	if (!m_isPaused) {

		CheckWinCon();

		if (m_isWin || m_isOutOfTime) {
			m_countdown.Stop();

			if (!m_newLevelTimer.isRunning()) {
				m_newLevelTimer.Reset();
				m_newLevelTimer.Start();
			}

			m_StartNewLevel = m_newLevelTimer.GetRemainingTime() < std::chrono::milliseconds{ 0 };

			if (m_StartNewLevel) {

				m_newLevelTimer.Stop();

				ResetWinCon();
				LoadNewLevel();
			}
		}
		else {
			for (auto actor : m_actors) {
				actor->Update(deltaTime);
			}
			m_herd.Update(deltaTime);
		}

	}

}

void CGame::Render()
{
	char buf[50];

	for (auto actor : m_actors) {
		actor->Render();
	}

	if(!m_isPaused ) {

		if (m_isWin || m_isOutOfTime) {

			if (!m_StartNewLevel) {

				DisplayEndLevelMessage();
			}
		}
	}
	else {
		sprintf(buf, "PAUSE");
		App::Print(950, 540, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	}

	DisplayInGameUI();

}

void CGame::Shutdown()
{
	while (!m_actors.empty()) {
		delete m_actors.back();
	}
}

void CGame::ProcessInputs()
{
	if (m_controller.CheckButton(XINPUT_GAMEPAD_START) && !m_isWin && !m_isOutOfTime) {
		if (m_isPaused) {
			m_isPaused = false;
			m_countdown.Start();
		}
		else {
			m_isPaused = true;
			m_countdown.Stop();
		}
	}
}

void CGame::LoadNewLevel()
{
	m_countdown.Reset();
	m_countdown.Start();
	CTileMap::Get().Load();
	m_herd.ResetSheep();
}

void CGame::CheckWinCon()
{
	m_isWin = m_herd.GetDeadSheepCount() == m_herd.GetSheepCount();
	m_isOutOfTime = m_countdown.GetRemainingTime() < std::chrono::milliseconds{ 0 };
}

void CGame::ResetWinCon()
{
	m_isWin = false;
	m_isOutOfTime = false;
	m_StartNewLevel = false;
}

void CGame::DisplayEndLevelMessage()
{
	char buf[50];
	int timeUntilNewLevel = (int)std::chrono::duration_cast<std::chrono::seconds>(m_newLevelTimer.GetRemainingTime()).count();

	if (m_isWin) {
		sprintf(buf, "Victory!");
		App::Print(950, 540, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		sprintf(buf, "New Level in %d", timeUntilNewLevel);
		App::Print(950, 520, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	}
	if (m_isOutOfTime) {
		sprintf(buf, "Out of time!");
		App::Print(950, 540, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		sprintf(buf, "New Level in %d", timeUntilNewLevel);
		App::Print(950, 520, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	}
}

void CGame::DisplayInGameUI()
{
	char buf[50];
	int activeSheep = m_herd.GetSheepCount();
	int deadSheep{ 0 };
	int timeRemaining{ 0 };

	deadSheep = m_herd.GetDeadSheepCount();
	timeRemaining = (int)std::chrono::duration_cast<std::chrono::seconds>(m_countdown.GetRemainingTime()).count();
	sprintf(buf, "Time Remaining: %d", timeRemaining);
	App::Print(20, 1000, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	sprintf(buf, " Gathered Sheep: %d/%d", deadSheep, activeSheep);
	App::Print(20, 950, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
	sprintf(buf, " Bark with A");
	App::Print(800, 50, buf, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
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

