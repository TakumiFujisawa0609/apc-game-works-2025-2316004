#pragma once
#include<DxLib.h>
#include<functional>
#include"../Common/Vector2F.h"
#include"../Common/Vector2.h"
class Easing
{
public:
	enum class EASING_TYPE
	{
		LERP,			//線形補間
		QUAD_IN,		//二次関数(だんだん早く)
		QUAD_OUT,		//二次関数(だんだん遅く)
		QUAD_IN_OUT,	//二次関数(スローインスローアウト)
		QUAD_OUT_IN,	//二次関数(ファストインファストアウト)
		CUBIC_IN,		//三次関数(だんだん早く)
		CUBIC_OUT,		//三次関数(だんだん遅く)
		EXPO,			//指数関数(〇次関数より若干緩やか)
		ELASTIC_OUT,	//ばね
		BOUNCE			//跳ねるような動き

	};

	//半分の割合
	static constexpr float HALF = 0.5f;

	//関数
	struct MATH_FUNC
	{
		//解の公式	a*(x-b)^2+c
		float accel = 1.0f;	//開き具合(公式のa)
		Vector2F graph_vertex = { 0.0f,0.0f };		//軸の頂点(b,c)

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

	//イージングセット
	void SetEasing(const float t, const EASING_TYPE type);

	/// @brief イージング計算
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @param type 使用するイージング種類
	/// @return 計算結果
	int EaseFunc(const int start, const int end, const float t,const EASING_TYPE type);
	float EaseFunc(const float start, const float end, const float t,const EASING_TYPE type);
	double EaseFunc(const double start, const double end, const float t,const EASING_TYPE type);
	Vector2F EaseFunc(const Vector2F& start, const Vector2F& end, const float t,const EASING_TYPE type);
	Vector2 EaseFunc(const Vector2& start, const Vector2& end, const float t, const EASING_TYPE type);
	VECTOR EaseFunc(const VECTOR& start, const VECTOR& end, const float t, const EASING_TYPE type);
	COLOR_F EaseFunc(const COLOR_F& start, const COLOR_F& end, const float t, const EASING_TYPE type);

	/// @brief 角度の最終計算(360度などの制限があるため、別で処理する)
	/// @param start 初期位置
	/// @param end 終了位置
	/// @param t 現在時間
	/// @param type 使用するイージング種類
	/// @return 計算結果
	float EaseFuncDeg(float& start, float& end, const float t, const EASING_TYPE type);
	double EaseFuncDeg(double& start, double& end, const float t, const EASING_TYPE type);
	float EaseFuncRad(float& start, float& end, const float t, const EASING_TYPE type);





private:
	//イージングの更新を格納
	std::function<float(const float)>easingUpdate_;


	/// @brief 線形補完
	/// @param t 時間
	/// @return 
	static float Lerp(const float t);
	//---------------------------------------------------------------------------------------------
	//二次関数系
	//---------------------------------------------------------------------------------------------
	// 二次関数的な動き(だんだん早く)
	static float EaseQuadIn(const float t);

	// 二次関数的な動き(だんだん遅く)
	float EaseQuadOut(const float t);


	//二次関数的な動き(スローインスローアウト的な)
	float EaseQuadInOut(const float t);


	//二次関数的な動き(ファストインファストアウト的な)
	float EaseQuadOutIn(const float t);
	//---------------------------------------------------------------------------------------------
	//三次関数(二次関数よりも緩やか)
	//---------------------------------------------------------------------------------------------

	// 三次関数の動き(だんだん早く)
	float EaseCubicIn(const float t);

	// 三次関数の動き(だんだん遅く)
	float EaseCubicOut(const float t);

	// 三次関数的な動き(スローインスローアウト的な)
	float EaseCubicInOut(const float start, const float end, const float t);

	//三次関数的な動き(ファストインファストアウト的な)
	float EaseCubicOutIn(const float start, const float end, const float t);

	//---------------------------------------------------------------------------------------------
	//指数関数
	//---------------------------------------------------------------------------------------------
	// 指数を決めてその動きをする
	float EaseExpo(const float t,const int expo=2);

	//後半にばねのような動き 
	float EaseOutElastic(const float t);

	//バウンドの動き 
	float EaseBounce(const float t);

	//外周を回る(お花の形)
	Vector2F EaseEpiCycloid(const Vector2F& start, const float t, const float halfRadiusNum = 4, const float smallRadius = 30);

	//内側を回る(とんがりお花)
	Vector2F EaseHypoCycloid(const Vector2F& start, const float t, const float halfRadiusNum = 4, const float smallRadius = 30);
};

