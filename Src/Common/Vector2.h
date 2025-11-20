#pragma once
#include "./Vector2F.h"

struct Vector2
{
	int x;
	int y;


	//Vector2F ToVector2F(void)
	//{
	//	Vector2F ret;

	//	ret.x = static_cast<float>(round(x));
	//	ret.y = static_cast<float>(round(y));

	//	return ret;
	//}

	//³‹K‰»‚·‚é
	Vector2 Normalize(void) const;

	//’·‚³‚ğ‹‚ß‚é
	float Length(void) const;

	//Vector2“¯m‚Ì”äŠr
	static bool IsVector2(const Vector2 value1, const Vector2 value2);

	//Vector2“¯m‚Ì’l‚ª“™‚µ‚¢‚©”äŠr
	static bool IsSameVector2(const Vector2 value1, const Vector2 value2);

	//Vector2“¯m‚Ì‰‰Z
	const Vector2 operator+(const Vector2 _value)const;
	void operator+=(const Vector2 _value);
	const Vector2 operator-(const Vector2 _value)const;
	void operator-=(const Vector2 _value);
	const Vector2 operator*(const Vector2 _value)const;
	void operator*=(const Vector2 _value);
	const Vector2 operator/(const Vector2 _value)const;
	void operator/=(const Vector2 _value);

	//ˆê‚Â‚Ì”‚Å‚Ì‰‰Z
	template<typename T>
	const Vector2 operator+(const T _value)const
	{
		return{ x + _value,y + _value };
	}
	template<typename T>
	void operator+=(const T _value)
	{
		x += _value;
		y += _value;

	}
	template<typename T>
	const Vector2 operator-(const T _value)const
	{
		return { static_cast<int>(x - _value),static_cast<int>(y - _value) };
	}
	template<typename T>
	void operator-=(const T _value)
	{
		x -= _value;
		y -= _value;
	}
	template<typename T>
	const Vector2 operator*(const T _value)const
	{
		return { static_cast<int>(x * _value),static_cast<int>(y * _value) };
	}
	template<typename T>
	void operator*=(const T _value)
	{
		x *= _value;
		y *= _value;
	}
	template<typename T>
	const Vector2 operator/(const T _value)const
	{
		return { static_cast<int>(x / _value),static_cast<int>(y / _value) };
	}
	template<typename T>
	void operator/=(const T _value)
	{
		x /= _value;
		y /= _value;
	}
};



