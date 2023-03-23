#include "stdafx.h"
#include "Vec2.h"
#include "Math.h"

float CVec2::length() const
{
	return Math::sqrt(m_x*m_x+m_y*m_y);
}

void CVec2::normalise()
{
	float length = this->length();
	m_x /= length;
	m_y /= length;
}
