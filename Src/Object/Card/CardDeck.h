#pragma once
#include<vector>
#include<memory>
#include "../../Common/Vector2.h"
#include "CardBase.h"

class CardBase;

class CardDeck
{

public:

	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	static constexpr int CARD_POWS[20] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
	//static constexpr int CARD_POWS[20] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};
	CardDeck(Vector2& _centerPos,int _playerNum);
	~CardDeck(void);

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief カード使用
	/// @param  
	void CardUseUpdate(void);

	/// @brief カードチャージ
	/// @param  
	void CardCharge(void);

	/// @brief 使用中のカードを消す
	/// @param  
	void EraseHandCard(void);

	/// @brief カード選択
	/// @param  
	void CardMoveLeft(void);
	void CardMoveRight(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);
	void Release(void);

	
	/// @brief 山札にカード追加
	/// @param _status 追加したいカードの強さ
	void AddDrawPile(const CardBase::CARD_STATUS& _status);

	/// @brief カードを使用札に移動
	/// @param  
	void MoveHandToCharge(void);
	//手札のカード取得
	//std::vector<std::weak_ptr<CardBase>> GetHand(void);

	/// @brief デッキからカードを引く
	/// @param  
	void DrawCardFromDeck(void);
	
	/// @brief カード失敗フラグ
	/// @param  
	/// @return true:負け　false:勝ち
	bool IsCardFailure(void);
	
	/// @brief カードの種類(アタックカードか魔法カードか)の取得
	/// @param  
	/// @return カードの種類
	std::vector<CardBase::CARD_TYPE> GetHandCardType(void);

	
	/// @brief 現在選択中のカードがリロードカードかを返す
	/// @param  
	/// @return 現在選択中のカード
	const CardBase::CARD_TYPE IsReloadCard(void);

	/// @brief リロードする
	/// @param  
	void Reload(void);
private:
	//メンバ関数
	//カードを選択したときの制限
	void CardMoveLimit(void);


	//札関連
	//初期札
	std::vector<std::unique_ptr<CardBase>>initDeck_;
	//山札
	std::vector<std::unique_ptr<CardBase>>drawPile_;
	//今使っているカード
	std::vector<std::unique_ptr<CardBase>>hand_;
	//チャージ中カード
	std::vector<std::unique_ptr<CardBase>>chargeCard_;
	//捨て札
	std::vector<std::unique_ptr<CardBase>>disCard_;

	//現在選択中のカード
	int currentNum_;
	//一つ先のカード番号
	int nextNum_;
	//ひとつ前
	int prevNum_;
	//カード使用者のプレイヤー番号
	int playerNum_;
	//現在選択中のカード中心座標
	Vector2& centerPos_;


};

