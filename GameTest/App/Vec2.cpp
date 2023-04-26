#include "stdafx.h"
#include "Vec2.h"
#include "Math.h"

float CVec2::Length() const
{
	return Math::sqrt(m_x*m_x+m_y*m_y);
}

void CVec2::Normalised()
{
	float Length = this->Length();
	m_x /= Length;
	m_y /= Length;
}

void CVec2::Rotate(float angle) {
	this->m_x = this->m_x * cos(angle) - this->m_y * sin(angle);
	this->m_y = this->m_x * sin(angle) + this->m_y * cos(angle);
}