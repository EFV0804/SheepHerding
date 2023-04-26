#pragma once

#include "Vec2.h"
#include "SimpleSprite.h"
#include <memory>
#include "Component.h"
#include "app.h"


class CActor
{
public:
	CActor(CVec2 position, int order);
	~CActor();
	void Update(float deltaTime);
	virtual void UpdateActor(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void Render();


	void GetPosition(CVec2& vec) { vec = m_position; }
	const CVec2& GetPosition() const { return m_position; }
	float GetScale() const { return m_scale; }
	float GetAngle() const { return m_angle; }
	int GetForce() const { return m_force; }
	const int GetOrder() const { return m_updateOrder; }
	void SetPosition(CVec2& position);
	void SetScale(float scale) { m_scale = scale; }
	void SetForce(int force) { m_force = force; }

	void AddComponent(CComponent* component);
	void RemoveComponent(CComponent* component);

	virtual void ButtonAction(int button);
	CVec2 m_orientation{ 0.0f,0.0f };


protected:
	CVec2 m_position;
	CVec2 m_refPosition;

	float m_scale{ 1.0f };
	float m_angle{ 0.0f };
	int m_force{ 0 };
	std::vector<CComponent*> m_components;
	int m_updateOrder;

};