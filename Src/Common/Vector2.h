#pragma once

class Vector2F;

class Vector2
{
public:

	int x;
	int y;

	// コンストラクタ
	Vector2(void);

	// コンストラクタ
	Vector2(int vX, int vY);

	// デストラクタ
	~Vector2(void);

	//Vector2へ変換する関数
	Vector2F ToVector2F(void);
	
	//正規化する
	Vector2 Normalize(void) const;

	//長さを求める
	float Length(void) const;

	//Vector2同士の比較
	static bool IsVector2(const Vector2 value1, const Vector2 value2);

	//Vector2同士の値が等しいか比較
	static bool IsSameVector2(const Vector2 value1, const Vector2 value2);

	//Vector2同士の足し算
	static Vector2 AddVector2(const Vector2 value1, const Vector2 value2);

	//Vector2同士の引き算
	static Vector2 SubVector2(const Vector2 value1, const Vector2 value2);

	//Vector2同士の掛け算
	static Vector2 MultiVector2(const Vector2 value1, const Vector2 value2);

};

