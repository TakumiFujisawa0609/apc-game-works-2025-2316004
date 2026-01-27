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

	//フルスクリーンフラグ
	const bool GetIsFullScreen(void) { return isFullScreen_; }

	//フルスクリーンの設定
	void SetIsFullScreen(const bool _isFullScreen);

	/// @brief カードデータの破棄
	/// @param  
	void ReleaseCardData(void);

	////プレイヤーのカードデータ保存
	//void AddPlayerCardData(const CardBase::CARD_STATUS _status);

	////エネミーのカードデータ保存
	//void AddEnemyCardData(const CardBase::CARD_STATUS _status);

	//キャラクターのカードデータ保管
	void AddCardData(const CHARACTER_TYPE _charaType, CardBase::CARD_STATUS _status);

	/// @brief カードデータの取得
	/// @param _charaType 取得したいキャラクター
	/// @return 初期カードデッキ
	std::vector<CardBase::CARD_STATUS> GetCardDatas(const CHARACTER_TYPE _charaType);

private:

	DataBank(void);
	DataBank(const DataBank& instance_) = default;
	~DataBank(void);
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする

	//フルスクリーンの設定
	bool isFullScreen_;

	//キャラクターカードデッキデータ
	std::unordered_map<CHARACTER_TYPE, std::vector<CardBase::CARD_STATUS>>characterCardDecks_;

	////プレイヤーのカードデッキ
	//std::vector<CardBase::CARD_STATUS> playerCardDatas_;

	////敵のカードデッキ
	//std::vector<CardBase::CARD_STATUS> enemyCardDatas_;
};

