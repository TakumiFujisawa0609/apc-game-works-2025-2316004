#pragma once
#include<vector>
#include<memory>
#include"../Object/ObjectBase.h"
#include "../../Common/Vector2.h"
#include "CardBase.h"

class CardBase;

class CardDeck:
	public ObjectBase
{

public:
	enum class DUELDECK_PETTERN
	{
		DUEL_PETTERN_1,
		DUEL_PETTERN_2,
		DUEL_PETTERN_3,
		END
	};


	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	static constexpr int CARD_POWS[20] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
	//static constexpr int CARD_POWS[20] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};
	CardDeck(Vector2& _centerPos,int _playerNum);
	~CardDeck(void);

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief カード使用
	/// @param  
	void CardUseUpdate(void);

	/// @brief カードチャージ
	/// @param  
	void CardCharge(void);
	

	/// @brief 使用中のカードを消す
	/// @param _isLose カード負けかどうか
	void EraseHandCard(const bool _isLose = false);

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

	void AddDuelDeck(const CardBase::CARD_STATUS& _status);

	/// @brief カードを使用札に移動
	/// @param  
	void MoveUsingCardToDrawPile(void);


	/// @brief デュエルデッキから使用札へ
	/// @param  
	void MoveUsingCardToDuelDeck(void);

	/// @brief デュエルデッキの解放
	/// @param  
	void ClearDuelDeck(void);


	//手札のカード取得
	//std::vector<std::weak_ptr<CardBase>> GetHand(void);

	/// @brief デッキからカードを引く
	/// @param  
	void DrawCardFromDeck(void);
	
	/// @brief カード失敗フラグ
	/// @param  
	/// @return true:負け
	bool IsCardFailure(void);

	/// @brief 何もカードが出ていない状態
	/// @param  
	/// @return 
	bool IsNone(void);

	//カード勝利フラグ	
	bool IsCardWin(void);
	
	/// @brief カードの種類(アタックカードか魔法カードか)の取得
	/// @param  
	/// @return カードの種類
	std::vector<CardBase::CARD_TYPE> GetHandCardType(void);

	
	/// @brief 現在選択中のカードがリロードカードかを返す
	/// @param  
	/// @return 現在選択中のカード
	const CardBase::CARD_TYPE GetDrawCardType(void);

	/// @brief リロードする
	/// @param  
	void Reload(void);

	/// @brief 使用中カードからチャージカードへ移動
	/// @param  
	void MoveChargeToUsingCard(void);

	//デュエルデッキをランダムにけっていする
	void DicideDuelDeck(void);

private:
	//メンバ定数
	
	static constexpr int DUEL_NUM_MAX = 3;
	static constexpr CardBase::CARD_STATUS DUEL_DECK_PETTERN_1[DUEL_NUM_MAX] = {
		{4,CardBase::CARD_TYPE::ATTACK},
		{6,CardBase::CARD_TYPE::ATTACK},
		{5,CardBase::CARD_TYPE::ATTACK}
	};
																				
	static constexpr CardBase::CARD_STATUS DUEL_DECK_PETTERN_2[DUEL_NUM_MAX] = {
		{2,CardBase::CARD_TYPE::ATTACK},
		{8,CardBase::CARD_TYPE::ATTACK},
		{6,CardBase::CARD_TYPE::ATTACK}
	};
	static constexpr CardBase::CARD_STATUS DUEL_DECK_PETTERN_3[DUEL_NUM_MAX] = { 
		{5,CardBase::CARD_TYPE::ATTACK},
		{7,CardBase::CARD_TYPE::ATTACK},
		{8,CardBase::CARD_TYPE::ATTACK}
	};
	//メンバ関数
	
	//カードを選択したときの制限
	void CardMoveLimit(void);


	//札関連
	//初期札
	std::vector<std::unique_ptr<CardBase>>initDeck_;
	//山札
	std::vector<std::unique_ptr<CardBase>>drawPile_;
	//今使っているカード
	std::vector<std::unique_ptr<CardBase>>usingCards_;
	//チャージ中カード
	std::vector<std::unique_ptr<CardBase>>chargeCard_;
	//捨て札
	std::vector<std::unique_ptr<CardBase>>disCard_;

	//敵用のデュエルデッキ
	std::vector<std::unique_ptr<CardBase>>enemyDuelDeck_;

	//現在選択中のカード
	int currentNum_;
	//一つ先のカード番号
	int nextNum_;
	//ひとつ前
	int prevNum_;
	//カード使用者のプレイヤー番号
	int playerNum_;
	//敵デュエルデッキの現在選択中番号
	int duelNo_;
	//現在選択中のカード中心座標
	Vector2& centerPos_;


};

