#pragma once

struct Vector2
{
	int x;
	int y;

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
};


//class Vector2F;
//
//
//
//class Vector2
//{
//public:
//
//	int x;
//	int y;
//
//	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//	Vector2(void);
//
//	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//	Vector2(int vX, int vY);
//
//	// ƒfƒXƒgƒ‰ƒNƒ^
//	~Vector2(void);
//
//	//Vector2‚Ö•ÏŠ·‚·‚éŠÖ”
//	Vector2F ToVector2F(void);
//	
//	//³‹K‰»‚·‚é
//	Vector2 Normalize(void) const;
//
//	//’·‚³‚ğ‹‚ß‚é
//	float Length(void) const;
//
//	//Vector2“¯m‚Ì”äŠr
//	static bool IsVector2(const Vector2 value1, const Vector2 value2);
//
//	//Vector2“¯m‚Ì’l‚ª“™‚µ‚¢‚©”äŠr
//	static bool IsSameVector2(const Vector2 value1, const Vector2 value2);
//
//	//Vector2“¯m‚Ì‘«‚µZ
//	static Vector2 AddVector2(const Vector2 value1, const Vector2 value2);
//
//	//Vector2“¯m‚Ìˆø‚«Z
//	static Vector2 SubVector2(const Vector2 value1, const Vector2 value2);
//
//	//Vector2“¯m‚ÌŠ|‚¯Z
//	static Vector2 MultiVector2(const Vector2 value1, const Vector2 value2);
//
//};

