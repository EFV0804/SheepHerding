#pragma once
#include <vector>
#include "Actor.h"
#include "PlayerActor.h"
#include "SheepActor.h"
#include "PhysicsManager.h"
#include "HerdManager.h"
#include "Timer.h"
#include "SimpleController.h"

class CGame {
public:
	CGame() = default;
	CGame(const CGame&) = delete;
	CGame& operator=(const CGame&) = delete;

	static CGame& Get() {
		static CGame instance;
		return instance;
	}

	void Init();
	void PreUpdate(float deltaTime);
	void Update(float deltaTime);
	void Render();
	void Shutdown();
	void ProcessInputs();

	void AddActor(CActor* actor);
	void RemoveActor(CActor* actor);

private:
	bool isPaused = false;
	const CController& m_controller{ CSimpleControllers::GetInstance().GetController(0) };
	std::vector<CActor*> m_actors;
	CHerdManager m_herd;
	CCustomTimer m_timer{};
	std::chrono::duration<double> m_levelTime{4 };
	CCustomTimer m_countdown{ true, m_levelTime };
};