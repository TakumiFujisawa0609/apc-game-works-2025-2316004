#pragma once

#include <string>
#include <unordered_map>
#include "../Object/Character/UIData/CharacterUIData.h"
#include "../Object/Card/CardBase.h"
#include "../../Template/Singleton.h"
class DataBank :public Singleton<DataBank>
{
	// シングルトンにだけ共有する
	friend class Singleton<DataBank>;

public:	

	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief プレイヤー人数を設定
	/// @param _num プレイヤー人数を設定
	/// @return 正しく設定できればtrue,問題が起きればfalse
	bool SetPlayerNum(int _num);
	
	/// @brief  プレイヤー人数を返す
	/// @param  
	/// @return プレイヤー人数
	int GetPlayerNum(void);

	/// @brief フルスクリーンフラグの取得
	/// @param  
	/// @return フルスクリーンフラグ
	const bool GetIsFullScreen(void) { return isFullScreen_; }

	/// @brief フルスクリーンの設定
	/// @param _isFullScreen フルスクリーンフラグ(true:フルスクリーン　false:ウィンドウ)
	void SetIsFullScreen(const bool _isFullScreen);

	/// @brief カードデータの破棄
	/// @param  
	void ReleaseCardData(void);

	/// @brief キャラクターのカードデータ保管
	/// @param _charaType キャラクター種類
	/// @param _status カードのステータス
	void AddCardData(const CHARACTER_TYPE _charaType, CardBase::CARD_STATUS _status);

	/// @brief カードデータの取得
	/// @param _charaType 取得したいキャラクター
	/// @return 初期カードデッキ
	std::vector<CardBase::CARD_STATUS> GetCardDatas(const CHARACTER_TYPE _charaType);

private:

	DataBank(void);
	DataBank(const DataBank& instance_) = delete;
	DataBank& operator=(const DataBank& _copy) = delete;

	~DataBank(void)override;
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする

	//フルスクリーンの設定
	bool isFullScreen_;

	//キャラクターカードデッキデータ
	std::unordered_map<CHARACTER_TYPE, std::vector<CardBase::CARD_STATUS>>characterCardDecks_;
};

