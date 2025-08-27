#pragma once
#include <DxLib.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../Common/Vector2.h"

class UtilityCommon
{
public:

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	//カラーコード
	static constexpr int BLACK = 0x000000;	//黒
	static constexpr int RED = 0xFF0000;	//赤
	static constexpr int BLUE = 0x0000FF;	//青
	static constexpr int YELLOW = 0xFFFF00;	//黄色
	static constexpr int GREEN = 0x008000;	//緑
	static constexpr int CYAN = 0x00FFFF;	//水色
	static constexpr int PINK = 0xFFC0CB;	//桃色
	static constexpr int ORANGE = 0xFFA500;	//オレンジ
	static constexpr int LIME = 0xADFF2F;	//黄緑
	static constexpr int PURPLE = 0x800080;	//紫
	static constexpr int WHITE = 0xFFFFFF;	//白
	static constexpr int GRAY = 0xBBBBBB;	//灰色
	static constexpr int BROWN = 0x8B4513;	//茶色

	// 四捨五入
	static int Round(float v);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);

	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// ベジェ曲線
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);

	//待機時間
	static bool IsTimeOver(float& totalTime, const float& waitTime);

	/// <summary>
	/// 範囲外の値を指定した範囲に収める
	/// </summary>
	/// <param name="index"></param>収めたい値
	/// <param name="max"></param>範囲の最大値
	/// <returns></returns>0からN-1の範囲に補正した値
	static int WrapIndex(int index, int max);

	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="start">開始値</param>
	/// <param name="change">変化量</param>
	/// /// <returns>計算結果</returns>
	static float EaseInQuad(const float time, const float start, const float change);

	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="start">開始値</param>
	/// <param name="change">変化量</param>
	/// <returns>計算結果</returns>
	static float EaseOutQuad(const float time, const float start, const float change);

	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>読み込んだデータ</returns>
	static std::vector<std::vector<int>> LoadCSVData(const std::wstring& filePath);
	
	/// <summary>
	/// stringオブジェクトから(SJIS)からwstringオブジェクトに変換
	/// </summary>
	/// <param name="str"></param>stringオブジェクト
	/// <returns></returns>wstringオブジェクト
	static std::wstring GetWStringFromString(const std::string& str);

};