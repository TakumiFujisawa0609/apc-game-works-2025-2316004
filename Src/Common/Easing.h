#pragma once
#include<DxLib.h>
#include"../Common/Vector2F.h"
#include"../Common/Vector2.h"
class Easing
{
public:
	//半分の割合
	static constexpr float HALF=0.5f;

	//関数
	struct MATH_FUNC	
	{
		//解の公式	a*(x-b)^2+c
		float accel=1.0f;	//開き具合(公式のa)
		Vector2F graph_vertex = {0.0f,0.0f};		//軸の頂点(b,c)

		//二次関数の計算
		float QuadFunc(const float& t)
		{
			float pow = powf(t - graph_vertex.x, 2.0f);
			return (accel * pow) + graph_vertex.y;
		}

		//三次関数の計算
		float CubicFunc(const float& t)
		{
			float pow = powf(t + graph_vertex.x, 3.0f);
			return (accel * pow) + graph_vertex.y;
		}
	};
	//最終計算
	static float EaseFunc(const float start, const float end, const float per);
	static Vector2F EaseFunc(const Vector2F& start, const Vector2F& end, const float per);
	static VECTOR EaseFunc(const VECTOR& start, const VECTOR& end, const float per);

	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start,float end,float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static Vector2F Lerp(const Vector2F& start, const Vector2F& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double& start, double& end, double t);
	static float LerpDeg(float& start, float& end, float t);
	static float LerpRad(float& start, float& end, float t);

	static float EaseInOut(const float start, const float end, const float t);



	/// @brief 後半にばねのような動き
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @param bounceNum バウンドの周期(値が小さい：細かく揺れる　　値が大きい：ゆっくり揺れる)
	/// @return 
	static float EaseOutElasticPer(const float t);
	static float EaseOutElastic(const float start, const float end, const float t);
	static Vector2F EaseOutElastic(const Vector2F& start, const Vector2F& end, float t);
	static VECTOR EaseOutElastic(const VECTOR& start, const VECTOR& end, const float& t);

	/// @brief バウンドの動き
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間 
	/// @return 
	static float EaseBounce(const float start, const float end, const float t);
	static Vector2F EaseBounce(const Vector2F& start, const Vector2F& end, const float& t);
	static VECTOR EaseBounce(const VECTOR& start, const VECTOR& end, const float& t);

	//---------------------------------------------------------------------------------------------
	//二次関数系
	//---------------------------------------------------------------------------------------------
	/// @brief 二次関数的な動き(だんだん早く)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseQuadIn(const float start, const float end, const float t);
	static Vector2F EaseQuadIn(const Vector2F& start, const Vector2F& end, const float t);
	static VECTOR EaseQuadIn(const VECTOR& start, const VECTOR& end, const float t);

	/// @brief 二次関数的な動き(だんだん遅く)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseQuadOut(const float start, const float end, const float t);
	static Vector2F EaseQuadOut(const Vector2F& start, const Vector2F& end, const float t);
	static VECTOR EaseQuadOut(const VECTOR& start, const VECTOR& end, const float t);

	/// @brief 二次関数的な動き(スローインスローアウト的な)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseQuadInOut(const float start, const float end, const float t);
	static Vector2F EaseQuadInOut(const Vector2F& start, const Vector2F& end, const float t);
	static VECTOR EaseQuadInOut(const VECTOR& start, const VECTOR& end, const float t);

	/// @brief 二次関数的な動き(ファストインファストアウト的な)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseQuadOutIn(const float start, const float end, const float t);
	static Vector2F EaseQuadOutIn(const Vector2F& start, const Vector2F& end, const float t);
	static VECTOR EaseQuadOutIn(const VECTOR& start, const VECTOR& end, const float t);
	//---------------------------------------------------------------------------------------------
	//三次関数(二次関数よりも緩やか)
	//---------------------------------------------------------------------------------------------

	/// @brief 三次関数の動き(だんだん早く)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseCubicIn(const float start, const float end, const float t);
	static Vector2F EaseCubicIn(const Vector2F& start, const Vector2F& end, const float t);
	static VECTOR EaseCubicIn(const VECTOR& start, const VECTOR& end, const float t);

	/// @brief 三次関数の動き(だんだん遅く)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseCubicOut(const float start, const float end, const float t);
	static Vector2F EaseCubicOut(const Vector2F start, const Vector2F end, const float t);
	static VECTOR EaseCubicOut(const VECTOR start, const VECTOR end, const float t);

	/// @brief 三次関数的な動き(スローインスローアウト的な)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseCubicInOut(const float start, const float end, const float t);

	/// @brief 三次関数的な動き(ファストインファストアウト的な)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @return 
	static float EaseCubicOutIn(const float start, const float end, const float t);

	//---------------------------------------------------------------------------------------------
	//指数関数
	//---------------------------------------------------------------------------------------------
	
	/// @brief 指数を決めてその動きをする
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @param expo べき乗(0<expo<1：EaseIn	expo>1：EaseOut)
	/// @return 
	static float EaseExpo(const float start, const float end, const float t,const float expo);
	static Vector2F EaseExpo(const Vector2F start, const Vector2F end, const float t,const float expo);
	static VECTOR EaseExpo(const VECTOR start, const VECTOR end, const float t,const float expo);
	static float EaseExpoInOut(const float start, const float end, const float t,const float expo);
	static Vector2F EaseExpoInOut(const Vector2F& start, const Vector2F& end, const float t, const float expo);
	static VECTOR EaseExpoInOut(const VECTOR& start, const VECTOR& end, const float t, const float expo);
	static float EaseExpoOutIn(const float start, const float end, const float t,const float expo);
	static float EaseExpoOutIn(const VECTOR& start, const VECTOR& end, const float& t, const float expo);

	
	//外周を回る(お花の形)
	static Vector2F EaseEpiCycloid(const Vector2F& start,const float t, const float halfRadiusNum = 4, const float smallRadius = 30);

	//内側を回る(とんがりお花)
	static Vector2F EaseHypoCycloid(const Vector2F& start, const float t, const float halfRadiusNum = 4, const float smallRadius = 30);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

};

