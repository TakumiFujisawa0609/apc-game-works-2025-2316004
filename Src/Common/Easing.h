#pragma once
#include<DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"
class Easing
{
public:
	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static Vector2F Lerp(const Vector2F& start, const Vector2F& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double& start, double& end, double t);
	static float LerpDeg(float& start, float& end, float t);
	static float LerpRad(float& start, float& end, float t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);


};

