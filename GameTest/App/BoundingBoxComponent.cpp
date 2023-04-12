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

void CBoundingBoxComponent::SetPosition(CVec2 position)
{
	m_owner.SetPosition(position);
}

const CVec2 CBoundingBoxComponent::GetPosition() const
{
	return m_owner.GetPosition();
}

CVec2 CBoundingBoxComponent::GetMinExtents() const
{
	float minX = m_owner.GetPosition().m_x;
	minX = (minX - ((m_size.m_x / 4.0f) * m_scale));
	float minY = m_owner.GetPosition().m_x;
	minY = (minY - ((m_size.m_y / 4.0f) * m_scale));
	return CVec2(minX, minY);
}

CVec2 CBoundingBoxComponent::GetMaxExtents() const
{
	float maxX = (GetMinExtents().m_x + ((m_size.m_x / 2.0f) * m_scale));
	float maxY = (GetMinExtents().m_y + ((m_size.m_y / 2.0f) * m_scale));
	return CVec2(maxX, maxY);
}

const CVec2 CBoundingBoxComponent::GetCenter() const
{
	CVec2 center = m_owner.GetPosition();
	center.m_x += m_size.m_x * 0.5;
	center.m_y += m_size.m_y * 0.5;
	return center;
}

const int CBoundingBoxComponent::GetForce() const
{
	return m_owner.GetForce();
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
