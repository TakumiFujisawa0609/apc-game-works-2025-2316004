#pragma once
#include <unordered_map>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../Template/Singleton.h"

class HpUIBase;
class CardUIBase;
class UIManager :public Singleton<UIManager>
{
	friend class Singleton<UIManager>;

public:

	/// @brief ロード
	/// @param  
	void Load(void);


	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 更新
	/// @param 
	void Update(void);
	
	/// @brief UIの描画
	/// @param  
	void Draw(void);
	
	/// @brief HPUIのHpを更新
	/// @param _charaType 更新したいキャラクター
	/// @param _hpData 更新するHp
	void RefreshHpUI(const CHARACTER_TYPE _charaType,const HP_DATA _hpData);

	/// @brief カードUIの取得
	/// @param _charaType 取得したいキャラクター
	/// @return 
	CardUIBase& GetCardUI(const CHARACTER_TYPE _charaType);
private:

	//ボタン
	int imgBtns_;

	//キャラHPUI
	std::unordered_map<CHARACTER_TYPE,std::unique_ptr<HpUIBase>>characterHpUI_;

	//カードUI
	std::unordered_map<CHARACTER_TYPE,std::unique_ptr<CardUIBase>>cardUI_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	UIManager(void);

	// デストラクタも同様
	~UIManager(void) = default;

	//HpUIの生成
	void CreateHpUI(void);

	//カードUIの生成
	void CreateCardUI(void);
};

