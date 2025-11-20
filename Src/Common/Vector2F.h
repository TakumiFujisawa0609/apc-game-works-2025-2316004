#pragma once
#include "./Vector2.h"

struct Vector2F
{
	//ŠeÀ•W
	float x;
	float y;

	////ƒxƒNƒgƒ‹ü‚ğì‚é
	//Vector2F CreateVectorLine(Vector2 value1, Vector2 value2);

	////Vector2‚Ö•ÏŠ·‚·‚éŠÖ”
	//Vector2 ToVector2(void)
	//{
	//	Vector2 ret;

	//	ret.x = static_cast<int>(roundf(x));
	//	ret.y = static_cast<int>(roundf(y));

	//	return ret;
	//}

	//Vector2F“¯m‚Ì”äŠr
	static bool IsVector2F(const Vector2F value1, const Vector2F value2);

	//Vector2F“¯m‚Ì‰‰Z
	const Vector2F operator+(const Vector2F _value)const;
	void operator+=(const Vector2F _value);
	const Vector2F operator-(const Vector2F _value)const;
	void operator-=(const Vector2F _value);
	const Vector2F operator*(const Vector2F _value)const;
	void operator*=(const Vector2F _value);
	const Vector2F operator/(const Vector2F _value)const;
	void operator/=(const Vector2F _value);

	//ˆê‚Â‚Ì”‚Å‚Ì‰‰Z
	template<typename T>
	const Vector2F operator+(const T _value)const
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
	const Vector2F operator-(const T _value)const
	{
		return { static_cast<float>(x - _value),static_cast<float>(y - _value) };
	}
	template<typename T>
	void operator-=(const T _value)
	{
		x -= _value;
		y -= _value;
	}
	template<typename T>
	const Vector2F operator*(const T _value)const
	{
		return { static_cast<float>(x * _value),static_cast<float>(y * _value) };
	}
	template<typename T>
	void operator*=(const T _value)
	{
		x *= _value;
		y *= _value;
	}
	template<typename T>
	const Vector2F operator/(const T _value)const
	{
		return { static_cast<float>(x / _value),static_cast<float>(y / _value) };
	}
	template<typename T>
	void operator/=(const T _value)
	{
		x /= _value;
		y /= _value;
	}

	//Vector2F“¯m‚Ì’l‚ª“¯‚¶‚©”äŠr
	static bool IsSameVector2F(const Vector2F value1, const Vector2F value2);

	////Vector2F“¯m‚Ì‰ÁZ
	//static Vector2F AddVector2F(Vector2F value1, Vector2F value2);

	////Vector2F“¯m‚ÌŒ¸Z
	//static Vector2F SubVector2F(Vector2F value1, Vector2F value2);

	////Vector2F“¯m‚ÌæZ
	//static Vector2F MulVector2F(Vector2F value1, Vector2F value2);

	////Vector2F“¯m‚ÌœZ
	//static Vector2F DivVector2F(Vector2F value1, Vector2F value2);
};


//class Vector2;
//
//class Vector2F
//{
//public:
//
//	//ƒfƒtƒHƒ‹ƒgƒRƒ“ƒXƒgƒ‰ƒNƒ^
//	Vector2F(void);
//
//	//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//	Vector2F(float ix, float iy);
//
//	//ƒfƒXƒgƒ‰ƒNƒ^
//	~Vector2F(void);
//
//	//ŠeÀ•W
//	float x;
//	float y;
//
//	//Vector2‚Ö•ÏŠ·‚·‚éŠÖ”
//	Vector2 ToVector2(void);
//
//	//ƒxƒNƒgƒ‹ü‚ğì‚é
//	Vector2F CreateVectorLine(Vector2 value1, Vector2 value2);
//
//	//Vector2F“¯m‚Ì”äŠr
//	static bool IsVector2F(const Vector2F value1, const Vector2F value2);
//
//	//Vector2F“¯m‚Ì’l‚ª“¯‚¶‚©”äŠr
//	static bool IsSameVector2F(const Vector2F value1, const Vector2F value2);
//
//	//Vector2F“¯m‚Ì‰ÁZ
//	static Vector2F AddVector2F(Vector2F value1, Vector2F value2);
//
//	//Vector2F“¯m‚ÌŒ¸Z
//	static Vector2F SubVector2F(Vector2F value1, Vector2F value2);
//
//	//Vector2F“¯m‚ÌæZ
//	static Vector2F MulVector2F(Vector2F value1, Vector2F value2);
//
//	//Vector2F“¯m‚ÌœZ
//	static Vector2F DivVector2F(Vector2F value1, Vector2F value2);
//
//
//
//private:
//};