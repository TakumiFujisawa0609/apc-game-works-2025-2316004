#pragma once
#include "../ObjectBase.h"
#include"../../Common/Vector2.h"
#include<memory>
class CardDeck;
class EnemyInput;
class Enemy :public ObjectBase
{
public:
	//定数
	static constexpr int RADIUS = 25; //敵のサイズ

	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;

	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };

	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;

	//格納するカードの強さ
	static constexpr int CARD_POWS[20] = { 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9 };

	inline const std::shared_ptr<CardDeck> GetHand(void)const { return deck_; }

	Enemy(void);
	~Enemy(void)override;
	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	Vector2 cardCenterPos_; //カードの中心座標
	//メンバ関数
	void DrawDebug(void);

	//敵の行動
	std::unique_ptr<EnemyInput>input_; //敵の入力クラス

	//メンバ変数
	std::shared_ptr<CardDeck>deck_;
};

