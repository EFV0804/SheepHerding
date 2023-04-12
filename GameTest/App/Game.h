#pragma once
#include <vector>
#include "Actor.h"
#include "PlayerActor.h"
#include "SheepActor.h"
#include "PhysicsManager.h"
#include "HerdManager.h"

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

	void AddActor(CActor* actor);
	void RemoveActor(CActor* actor);

private:
	std::vector<CActor*> m_actors;
	CPhysicsManager m_physics;
	CHerdManager m_herd;
};