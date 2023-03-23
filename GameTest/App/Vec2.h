#ifndef _VEC2_H
#define _VEC2_H

class CVec2
{
public:
	CVec2() = delete;
	CVec2(float x, float y) : m_x{ x }, m_y{ y } { };

	void GetComponents(float& m_x, float& m_y) { m_x = m_x; m_y = m_y; }
	void SetComponents(float m_x, float m_y) { m_x = m_x; m_y = m_y; }
	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }
	
	float length() const;
	void normalise();

	static CVec2 normalise(const CVec2& vec)
	{
		CVec2 temp = vec;
		temp.normalise();
		return temp;
	}

	static float dot(const CVec2& a, const CVec2& b)
	{
		return a.m_x * b.m_x + a.m_y * b.m_y;
	}

	static CVec2 lerp(const CVec2& a, const CVec2& b, float f)
	{
		return CVec2(a + f * (b - a));
	}

	CVec2& operator+=(const CVec2& right)
	{
		m_x += right.m_x;
		m_y += right.m_y;
		return *this;
	}

	friend CVec2 operator+(const CVec2& left, const CVec2& right)
	{
		return CVec2(left.m_x + right.m_x, left.m_y + right.m_y);
	}

	CVec2& operator-=(const CVec2& right)
	{
		m_x -= right.m_x;
		m_y -= right.m_y;
		return *this;
	}

	friend CVec2 operator-(const CVec2& left, const CVec2& right)
	{
		return CVec2(left.m_x - right.m_x, left.m_y - right.m_y);
	}

	CVec2& operator*=(float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;
		return *this;
	}

	friend CVec2 operator*(const CVec2& vec, float scalar)
	{
		return CVec2(vec.m_x * scalar, vec.m_y * scalar);
	}

	friend CVec2 operator*(float scalar, const CVec2& vec)
	{
		return CVec2(vec.m_x * scalar, vec.m_y * scalar);
	}

private:

	float m_x{ 0 };
	float m_y{ 0 };

};

#endif //_VEC2_H