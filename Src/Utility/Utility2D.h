#pragma once
#include "../Common/Vector2.h"

class Utility2D
{
public:

	//ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static int SqrMagnitude(const Vector2& v);
	static double Distance(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// ワールド座標をマップ座標へ
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <param name="mapSize">マップチップ等のサイズ</param>
	/// <returns>マップ座標</returns>
	static Vector2 WorldToMapPos(const Vector2& worldPos, const Vector2& mapSize);

	/// <summary>
	/// マップ座標をワールド座標に
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <param name="mapSize">マップチップ等のサイズ</param>
	/// <returns>ワールド座標</returns>
	static Vector2 MapToWorldPos(const Vector2& mapPos, const Vector2& mapSize);

	/// <summary>
	/// 円同士の衝突判定
	/// </summary>
	/// <param name="circlePos1">円座標1</param>
	/// <param name="radius1">円半径1</param>
	/// <param name="circlePos2">円座標2</param>
	/// <param name="radius2">円半径2</param>
	/// <returns>衝突してたらtrue, そうじゃない場合false</returns>
	static bool IsHitCircles(const Vector2& circlePos1, const float radius1, const Vector2& circlePos2, const float radius2);

};

