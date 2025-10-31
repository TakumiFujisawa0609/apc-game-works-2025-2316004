#pragma once
#include<memory>
#include "../Base/CharacterBase.h"
#include"../../Card/CardBase.h"
#include"../../../Common/Vector2.h"

class CardDeck;
class EnemyInput;
class ActionController;
class AnimationController;

class Idle;
class Run;
class Jump;
class React;
class EnemyCardAction;
class Enemy :public CharacterBase
{
public:
	using CARD_TYPE = CardBase::CARD_TYPE;
	//定数
	static constexpr int RADIUS = 25; //敵のサイズ
	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;
	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	//格納するカードの強さ
	static constexpr CardBase::CARD_STATUS CARD_POWS[20] = {
		{0,CARD_TYPE::ATTACK},
		{1,CARD_TYPE::ATTACK},
		{2,CARD_TYPE::ATTACK},
		{3,CARD_TYPE::ATTACK},
		{4,CARD_TYPE::ATTACK},
		{5,CARD_TYPE::ATTACK},
		{6,CARD_TYPE::ATTACK},
		{7,CARD_TYPE::ATTACK},
		{8,CARD_TYPE::ATTACK},
		{9,CARD_TYPE::ATTACK},
		{0,CARD_TYPE::ATTACK},
		{1,CARD_TYPE::ATTACK},
		{2,CARD_TYPE::ATTACK},
		{3,CARD_TYPE::ATTACK},
		{4,CARD_TYPE::ATTACK},
		{5,CARD_TYPE::ATTACK},
		{6,CARD_TYPE::ATTACK},
		{7,CARD_TYPE::ATTACK},
		{8,CARD_TYPE::ATTACK},
		{9,CARD_TYPE::ATTACK}
	};



	const std::shared_ptr<CardDeck> GetHand(void)const { return deck_; }

	Enemy(CharacterBase& _playerChara);
	~Enemy(void)override;
	void Load(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// オブジェクトに当たった時の処理
	/// </summary>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// <summary>
	/// 入力方向に応じて方向を決める
	/// </summary>
	/// <param name=""></param>
	void MoveDirFronInput(void)override;
	/// <summary>
	/// ゴール角度をセット
	/// </summary>
	/// <param name="_deg"></param>
	void SetGoalRotate(const double _deg)override;

private:

	//定数
	//プレイヤーの腰のフレーム番号
	static constexpr int SPINE_FRAME_NO = 1;
	//敵番号(デッキで判定する用)
	static constexpr int ENEMY_NUM = 1;
	Vector2 cardCenterPos_; //カードの中心座標
	CharacterBase& playerChara_;	//プレイヤー情報

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

	//メンバ関数
	


	//デバッグ用で
};

