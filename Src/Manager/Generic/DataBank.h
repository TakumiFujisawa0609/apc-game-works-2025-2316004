#pragma once

#include <string>
#include "../Object/Card/CardBase.h"
class DataBank
{
public:	
	/// @brief 明示的にインステンスを生成する
	/// @param  
	static void CreateInstance(void);
	
	/// @brief 静的インスタンスの取得
	/// @param  
	/// @return インスタンスを返す
	static DataBank& GetInstance(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 解放処理
	/// @param  
	void Destroy(void);
	
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

	//プレイヤーのカードデータ保存
	void AddPlayerCardData(const CardBase::CARD_STATUS _status);

	//エネミーのカードデータ保存
	void AddEnemyCardData(const CardBase::CARD_STATUS _status);



private:


	static DataBank* instance_;
	DataBank(void);
	DataBank(const DataBank& instance_) = default;
	~DataBank(void);
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする

	//フルスクリーンの設定
	bool isFullScreen_;

	//プレイヤーのカードデッキ
	std::vector<CardBase::CARD_STATUS> playerCardDatas_;

	//敵のカードデッキ
	std::vector<CardBase::CARD_STATUS> enemyCardDatas_;
};

