#include <cmath>
#include "Vector2.h"
#include "Vector2F.h"

// コンストラクタ
Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

// コンストラクタ
Vector2::Vector2(int vX, int vY)
{
	x = vX;
	y = vY;
}

Vector2::~Vector2(void)
{
}

Vector2F Vector2::ToVector2F(void)
{
	Vector2F ret;

	ret.x = static_cast<float>(round(x));
	ret.y = static_cast<float>(round(y));

	return ret;
}

Vector2 Vector2::Normalize(void) const
{
	float len = Length();
	return (len != 0) ? Vector2(x / len, y / len) : Vector2(0, 0);
}

float Vector2::Length(void) const
{
	return std::sqrt(x * x + y * y);
}

bool Vector2::IsVector2(const Vector2 value1, const Vector2 value2)
{
	if (value1.x < value2.x &&
		value1.y < value2.y)
	{
		return true;
	}

	return false;
}

bool Vector2::IsSameVector2(const Vector2 value1, const Vector2 value2)
{
	if (value1.x == value2.x &&
		value1.y == value2.y)
	{
		return true;
	}

	return false;
}

Vector2 Vector2::AddVector2(const Vector2 value1, const Vector2 value2)
{
	Vector2 ret;
	ret.x = value1.x + value2.x;
	ret.y = value1.y + value2.y;
	return ret;
}

Vector2 Vector2::SubVector2(const Vector2 value1, const Vector2 value2)
{
	Vector2 ret;
	ret.x = value1.x - value2.x;
	ret.y = value1.y - value2.y;
	return ret;
}

Vector2 Vector2::MultiVector2(const Vector2 value1, const Vector2 value2)
{
	Vector2 ret;
	ret.x = value1.x * value2.x;
	ret.y = value1.y * value2.y;
	return ret;
}