#pragma once
#include <memory>

class CActor;

class CComponent
{
public:
	CComponent(CActor* owner, int updateOrder = 100, bool isRenderable = false);
	CComponent() = delete;
	virtual ~CComponent();
	CComponent(const CComponent&) = delete;
	CComponent& operator=(const CComponent&) = delete;

	virtual void Update(float deltaTime);
	virtual void Render();
	
	const int GetOrder() const { return m_updateOrder; }
	const bool IsRenderable() const { return m_isRenderable; }

protected:
	CActor& m_owner;
	int m_updateOrder;
	bool m_isRenderable;
};

