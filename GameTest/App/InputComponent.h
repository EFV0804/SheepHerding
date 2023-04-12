#pragma once
#include <windows.h> 
#include "SimpleController.h"
#include "Component.h"

class CActor;

class CInputComponent:
    public CComponent 
{
public:
    CInputComponent(CActor* owner, int updateOrder);
    CInputComponent(const CInputComponent&) = delete;
    CInputComponent() = delete;
    CInputComponent& operator=(const CInputComponent&) = delete;

    void Update(float deltaTime);

    const float GetSpeed() const { return m_speed; }
    void SetSpeed(float speed) { m_speed = speed; }

private:
    float m_speed{ 0.02f };
    const CController& m_controller;
};