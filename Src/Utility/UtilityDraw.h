#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class UtilityDraw
{
public:

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	/// <summary>
	/// 点線を描画する
	/// </summary>
	/// <param name="sPos">始点</param>
	/// <param name="ePos">終点</param>
	/// <param name="color">色</param>
	/// <param name="len">間隔</param>
	static void DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len = 50.0f);
};

