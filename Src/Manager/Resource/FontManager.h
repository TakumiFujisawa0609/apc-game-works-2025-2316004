#pragma once
#include <string>

class FontManager
{
public:

	enum class FONT_TYPE
	{
		DOT,
		BOKUTATI,
		MAX
	};

	//フォント種類
	static constexpr int FONT_TYPES = static_cast<int>(FONT_TYPE::MAX);

	//フォントネーム(cppで設定)
	static const std::wstring FONT_DOT;
	static const std::wstring FONT_BOKUTATI;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FontManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FontManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Destroy();

private:

	//登録用パス
	std::wstring fontPath_[FONT_TYPES];
};

