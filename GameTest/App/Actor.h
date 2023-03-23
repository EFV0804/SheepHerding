#ifndef _ACTOR_H
#define _ACTOR_H

#include "Vec2.h"
#include "SimpleSprite.h"

class CActor
{
public:
	CActor(CVec2 position) : m_position{ position } {};
	CActor(const CActor&) = delete;
	CActor& operator=(const CActor&) = delete;
	~CActor() { delete m_sprite; }

	void Update(float deltaTime);
	void Draw();

	CSimpleSprite* GetSprite() { return m_sprite; }
	void SetSprite(CSimpleSprite* sprite);
	void GetPosition(CVec2 vec) { vec = m_position; }
	float GetScale() const { return m_scale; }
	float GetAngle() const { return m_angle; }

	void SetPosition(CVec2 position) { m_position = position; }

private:
	CVec2 m_position;
	float m_scale{ 1.0f };
	float m_angle{ 0.0f };
	CSimpleSprite* m_sprite{};

};

#endif //_ACTOR_H