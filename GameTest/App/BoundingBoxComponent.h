#pragma once
#include "Component.h"
#include "Vec2.h"

class CActor;

class CBoundingBoxComponent :
    public CComponent
{
public:
    CBoundingBoxComponent(CActor* owner, int updateOrder, CVec2 size);
    CBoundingBoxComponent() = delete;
    CBoundingBoxComponent(const CBoundingBoxComponent&) = delete;

    void SetPosition(CVec2 position);
    void SetSize(const CVec2 vec) { m_size = vec; }
    CVec2 GetSize() { return m_size; }
    const float GetScale() const { return m_scale; }
    const CVec2 GetPosition() const;
    CVec2 GetMinExtents() const;
    CVec2 GetMaxExtents() const;
    const CVec2 GetCenter() const;
    const int GetForce() const;
    void Render();

private:
    CVec2 m_size{ 0.0f,0.0f };
    float m_scale{ 1.0f };
};

