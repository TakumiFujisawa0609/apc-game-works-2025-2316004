#pragma once
#include <DxLib.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"

class UtilityCommon
{
public:

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	//カラーコード
	static constexpr unsigned int BLACK = 0x000000;	//黒
	static constexpr unsigned int RED = 0xFF0000;	//赤
	static constexpr unsigned int BLUE = 0x0000FF;	//青
	static constexpr unsigned int YELLOW = 0xFFFF00;	//黄色
	static constexpr unsigned int GREEN = 0x008000;	//緑
	static constexpr unsigned int CYAN = 0x00FFFF;	//水色
	static constexpr unsigned int PINK = 0xFFC0CB;	//桃色
	static constexpr unsigned int ORANGE = 0xFFA500;	//オレンジ
	static constexpr unsigned int LIME = 0xADFF2F;	//黄緑
	static constexpr unsigned int PURPLE = 0x800080;	//紫
	static constexpr unsigned int WHITE = 0xFFFFFF;	//白
	static constexpr unsigned int GRAY = 0xBBBBBB;	//灰色
	static constexpr unsigned int BROWN = 0x8B4513;	//茶色

	//固定デルタタイム
	static constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f;
	//パーセント
	static constexpr float PERCENT_MAX = 1.0f;

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



	//// 線形補間
	//static int Lerp(int start, int end, float t);
	//static float Lerp(float start, float end, float t);
	//static double Lerp(double start, double end, double t);
	//static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	//static Vector2F Lerp(const Vector2F& start, const Vector2F& end, float t);
	//static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	//// 角度の線形補間
	//static double LerpDeg(double& start, double& end, double t);
	//static float LerpDeg(float& start, float& end, float t);
	//static float LerpRad(float& start, float& end, float t);

	//// 色の線形補間
	//static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	//// ベジェ曲線
	//static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	//static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);

	//待機時間
	static bool IsTimeOver(float& totalTime, const float& waitTime);
	
	/// @brief 範囲外の値を指定した範囲に収める
	/// @param index 収めたい値
	/// @param max 範囲の最大値
	/// @return 0からN-1の範囲に補正した値
	static int WrapIndex(int index, int max);
	
	/// @brief イーズイン
	/// @param time 時間
	/// @param start 開始値
	/// @param change 変化量
	/// @return 計算結果
	static float EaseInQuad(const float time, const float start, const float change);

	/// @brief イーズアウト
	/// @param time 時間
	/// @param start 開始値
	/// @param change 変化量
	/// @return 計算結果
	static float EaseOutQuad(const float time, const float start, const float change);
	
	/// @brief CSVファイルの読み込み
	/// @param filePath ファイルパス
	/// @return 読み込んだデータ
	static std::vector<std::vector<int>> LoadCSVData(const std::wstring& filePath);
	
	/// @brief stringオブジェクトから(SJIS)からwstringオブジェクトに変換
	/// @param str stringオブジェクト
	/// @return wstringオブジェクト
	static std::wstring GetWStringFromString(const std::string& str);



};