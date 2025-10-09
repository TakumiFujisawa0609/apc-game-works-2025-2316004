#include <cmath>
#include "Vector2.h"
#include "Vector2F.h"


////デフォルトコンストラクタ
//Vector2F::Vector2F(void)
//{
//	x = 0.0F;
//	y = 0.0F;
//}
//
//
////コンストラクタ
//Vector2F::Vector2F(float ix, float iy)
//{
//	x = ix;
//	y = iy;
//}
//
//
////デストラクタ
//Vector2F::~Vector2F(void)
//{
//
//}


//Vector2へ変換する関数
Vector2 ToVector2(Vector2F _vec)
{
	Vector2 ret;

	ret.x = static_cast<int>(roundf(_vec.x));
	ret.y = static_cast<int>(roundf(_vec.y));

	return ret;
}

//ベクトル線を作る
Vector2F CreateVectorLine(Vector2 value1, Vector2 value2)
{
	return Vector2F(static_cast<float>(value1.x - value2.x),
		static_cast<float>(value1.y - value2.y));
}

//Vector2F同士の比較
bool Vector2F::IsVector2F(Vector2F value1, Vector2F value2)
{
	if (value1.x < value2.x &&
		value1.y < value2.y)
	{
		return true;
	}

	return false;
}

bool Vector2F::IsSameVector2F(const Vector2F value1, const Vector2F value2)
{
	if (value1.x == value2.x && value1.y == value2.y)
	{
		return true;
	}
	return false;
}

//const Vector2F operator+(const T _value)const;
//template<typename T>
//void operator+=(const T _value)const;
//template<typename T>
//const Vector2F operator-(const T _value)const;
//template<typename T>
//void operator-=(const T _value)const;
//template<typename T>
//const Vector2F operator*(const T _value)const;
//template<typename T>
//void operator*=(const T _value)const;
//template<typename T>
//const Vector2F operator-(const T _value)const;
//template<typename T>
//void operator-=(const T _value)const;

//Vector2F Vector2F::AddVector2F(Vector2F value1, Vector2F value2)
//{
//	Vector2F ret;
//	ret.x = value1.x + value2.x;
//	ret.y = value1.y + value2.y;
//	return ret;
//}
//
//Vector2F Vector2F::SubVector2F(Vector2F value1, Vector2F value2)
//{
//	Vector2F ret;
//	ret.x = value1.x - value2.x;
//	ret.y = value1.y - value2.y;
//	return ret;
//}
//
//Vector2F Vector2F::MulVector2F(Vector2F value1, Vector2F value2)
//{
//	Vector2F ret;
//	ret.x = value1.x * value2.x;
//	ret.y = value1.y * value2.y;
//	return ret;
//}
//
//Vector2F Vector2F::DivVector2F(Vector2F value1, Vector2F value2)
//{
//	Vector2F ret;
//	ret.x = value1.x / value2.x;
//	ret.y = value1.y / value2.y;
//	return ret;
//}

const Vector2F Vector2F::operator+(const Vector2F _value)const
{
	return { x + _value.x,y + _value.y };
}
void Vector2F::operator+=(const Vector2F _value)
{
	x += _value.x;
	y += _value.y;
}
const Vector2F Vector2F::operator-(const Vector2F _value)const
{
	return { x - _value.x,y - _value.y };
}
void Vector2F::operator-=(const Vector2F _value)
{
	x -= _value.x;
	y -= _value.y;
}
const Vector2F Vector2F::operator*(const Vector2F _value)const
{
	return { x * _value.x,y * _value.y };
}
void Vector2F::operator*=(const Vector2F _value)
{
	x*= _value.x;
	y*= _value.y;
}
const Vector2F Vector2F::operator/(const Vector2F _value)const
{
	return { x / _value.x,y / _value.y };
}
void Vector2F::operator/=(const Vector2F _value)
{
	x /= _value.x;
	y /= _value.y;
}