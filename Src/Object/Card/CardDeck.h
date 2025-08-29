#pragma once
#include<vector>
#include<memory>
#include "../../Common/Vector2.h"
class CardBase;
class CardDeck
{
public:
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	static constexpr int CARD_POWS[20] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
	//static constexpr int CARD_POWS[20] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};
	CardDeck(Vector2& _centerPos);
	~CardDeck(void);

	//初期化
	void Init(void);
	//カード使用
	void CardUse(void);
	//カードチャージ
	void CardCharge(void);
	//カードを捨て札に
	void DisCard(void);
	//カード選択
	void CardMoveLeft(void);
	void CardMoveRight(void);
	void Update(void);
	//描画
	void Draw(void);
	void Release(void);

	/// <summary>
	/// 山札にカード追加
	/// </summary>
	/// <param name="_pow">追加したいカードの強さ</param>
	void AddDrawPile(const int _pow);

	//手札のカード取得
	std::vector<std::weak_ptr<CardBase>> GetHand(void);

private:
	//メンバ関数
	//カードを選択したときの制限
	void CardMoveLimit(void);

	//札関連
	//山札
	//std::vector<std::unique_ptr<CardBase>>drawPile_;
	std::vector<std::unique_ptr<CardBase>>drawPile_;
	//今使っているカード
	std::vector<std::shared_ptr<CardBase>>hand_;
	//std::vector<CardBase&>hand_;
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

	//現在選択中のカード中心座標
	Vector2& centerPos_;
};

