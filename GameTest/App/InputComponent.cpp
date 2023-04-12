#include "stdafx.h"
#include "InputComponent.h"
#include "Vec2.h"
#include "Actor.h"

CInputComponent::CInputComponent(CActor* owner, int updateOrder): 
	CComponent(owner, updateOrder),
	m_controller{ CSimpleControllers::GetInstance().GetController(0) }
{
	
}

void CInputComponent::Update(float deltaTime)
{
	if (m_controller.GetLeftThumbStickX() > 0.5f)
	{
		CVec2 vec(0.0f, 0.0f);
		m_owner.GetPosition(vec);
		vec.m_x += m_speed*(0.02*deltaTime);
		m_owner.SetPosition(vec);
	}
	if (m_controller.GetLeftThumbStickX() < -0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		CVec2 vec(0.0f, 0.0f);
		m_owner.GetPosition(vec);
		vec.m_x -= m_speed * (0.02 * deltaTime);
		m_owner.SetPosition(vec);
	}
	if (m_controller.GetLeftThumbStickY() > 0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		CVec2 vec(0.0f, 0.0f);
		m_owner.GetPosition(vec);
		vec.m_y += m_speed * (0.02 * deltaTime);
		m_owner.SetPosition(vec);
	}
	if (m_controller.GetLeftThumbStickY() < -0.5f)
	{
		//player->SetAnimation(ANIM_BACKWARDS);
		CVec2 vec(0.0f, 0.0f);
		m_owner.GetPosition(vec);
		vec.m_y -= m_speed * (0.02 * deltaTime);
		m_owner.SetPosition(vec);
	}
}
