#include "stdafx.h"
#include "BoundingBoxComponent.h"
#include "Actor.h"
#include "Math.h"
#include "app.h"

CBoundingBoxComponent::CBoundingBoxComponent(CActor* owner, int updateOrder, CVec2 size):
	CComponent(owner, updateOrder, true),
	m_size {size}
{

}

const CVec2 CBoundingBoxComponent::GetPosition() const
{
	return m_owner.GetPosition();
}

void CBoundingBoxComponent::Render()
{
	CVec2 position{ 0.0f,0.0f };
	m_owner.GetPosition(position);
	float angle = m_owner.GetAngle();
	angle = Math::toDegrees(angle);
	float scale = m_owner.GetScale();

	float aMinX = (position.m_x - ((m_size.m_x / 4.0f) * scale));
	float aMinY = (position.m_y - ((m_size.m_y / 4.0f) * scale));
	float aMaxX = (aMinX + ((m_size.m_x / 2.0f) * scale));
	float aMaxY = (aMinY + ((m_size.m_y / 2.0f) * scale));

	App::DrawLine(aMinX, aMinY, aMaxX, aMinY, 1.0f, 0.0f, 0.0f);
	App::DrawLine(aMinX, aMinY, aMinX, aMaxY, 1.0f, 0.0f, 0.0f);
	App::DrawLine(aMaxX, aMinY, aMaxX, aMaxY, 1.0f, 0.0f, 0.0f);
	App::DrawLine(aMinX, aMaxY, aMaxX, aMaxY, 1.0f, 0.0f, 0.0f);
}
