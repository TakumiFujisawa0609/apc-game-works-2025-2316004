#pragma once
#include <unordered_map>
#include "../Template/Singleton.h"

class HpUIBase;
class UIManager :public Singleton<UIManager>
{
	friend class Singleton<UIManager>;

public:
	enum class CHARA_TYPE
	{
		PLAYER,
		ENEMY,
		MAX
	};

	// UIの初期化
	void Init();
	// UIの更新
	void Update();
	// UIの描画
	void Draw();
	// UIの解放
	void Release();
private:

	//ボタン
	int imgBtns_;

	//キャラHPUI
	std::unordered_map<CHARA_TYPE,std::unique_ptr<HpUIBase>>characterHpUI_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UIManager(void);

	// デストラクタも同様
	~UIManager(void) = default;
};

