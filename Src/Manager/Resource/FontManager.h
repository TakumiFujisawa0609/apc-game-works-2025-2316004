#pragma once
#include <string>

class FontManager
{
public:

	enum class FONT_TYPE
	{
		DOT,
		BOKUTATI,
		APRIL_GOTHIC_ONE,
		MAX
	};

	//フォント種類
	static constexpr int FONT_TYPES = static_cast<int>(FONT_TYPE::MAX);

	//フォントネーム(cppで設定。プロパティ→詳細のタイトルを入れること)
	static const std::wstring FONT_DOT;
	static const std::wstring FONT_BOKUTATI;
	static const std::wstring FONT_APRIL_GOTHIC;
	
	/// @brief コンストラクタ
	/// @param  
	FontManager(void);

	/// @brief デストラクタ
	/// @param  
	~FontManager(void) = default;

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 解放処理
	/// @param  
	void Destroy(void);

private:

	//登録用パス
	std::wstring fontPath_[FONT_TYPES];
};

