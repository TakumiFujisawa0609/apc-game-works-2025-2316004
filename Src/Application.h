#pragma once
#include <string>
#include "Template/Singleton.h"

class FpsControl;
class FontManager;

class Application : public Singleton<Application>
{
	friend class Singleton<Application>; 

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	// スクリーンサイズハーフ
	static constexpr int SCREEN_HALF_X = SCREEN_SIZE_X / 2;
	static constexpr int SCREEN_HALF_Y = SCREEN_SIZE_Y / 2;

	// データパス関連
	//-------------------------------------------
	static const std::wstring PATH_IMAGE;
	static const std::wstring PATH_MODEL;
	static const std::wstring PATH_ANIM;
	static const std::wstring PATH_EFFECT;
	static const std::wstring PATH_SOUND;
	static const std::wstring PATH_FONT;
	static const std::wstring PATH_TEXT;
	static const std::wstring PATH_JSON;
	static const std::wstring PATH_CSV;
	static const std::wstring PATH_SHADER;
	//-------------------------------------------

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	/// <returns>初期化成功の場合true,そうじゃない場合false</returns>
	bool Init();

	/// <summary>
	/// ゲームの実行ループ
	/// </summary>
	/// <param name=""></param>
	void Run();

	/// <summary>
	/// リソースの破棄
	/// </summary>
	/// <param name=""></param>
	/// <returns>解放成功の場合true,そうじゃない場合false</returns>
	bool Release();

private:

	//FPS
	std::unique_ptr<FpsControl> fps_;

	//フォント
	std::unique_ptr<FontManager> fontMng_;

	//コンストラクタ
	Application();

	//デストラクタ
	~Application() = default;

	// Effekseerの初期化
	void InitEffekseer();

};