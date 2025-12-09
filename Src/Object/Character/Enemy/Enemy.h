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

	//敵の大きさの倍率
	static constexpr float MODEL_SIZE_MULTIPLITER = 5.0f;

	//敵の大きさ
	static constexpr VECTOR MODEL_SCL = { MODEL_SIZE_MULTIPLITER,MODEL_SIZE_MULTIPLITER,MODEL_SIZE_MULTIPLITER };
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 3;

	//ステータス関連
	static constexpr float MOVE_SPEED = 15.0f;		//移動スピード
	static constexpr float MAX_HP = 200.0f;			//体力
	static constexpr float MAX_ATK = 50.0f;			//攻撃力
	static constexpr float MAX_DEF = 100.0f;		//防御力
	//アニメーション速度
	static constexpr float ANIM_SPEED = 10.0f;
	//格納するカードの強さ
	static constexpr CardBase::CARD_STATUS CARD_POWS[CARD_NUM_MAX] = {
		{0,CARD_TYPE::ATTACK},
		{1,CARD_TYPE::ATTACK},
		{2,CARD_TYPE::ATTACK},
		//{3,CARD_TYPE::ATTACK},
		//{4,CARD_TYPE::ATTACK},
		//{5,CARD_TYPE::ATTACK},
		//{6,CARD_TYPE::ATTACK},
		//{7,CARD_TYPE::ATTACK},
		//{8,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{0,CARD_TYPE::ATTACK},
		//{1,CARD_TYPE::ATTACK},
		//{2,CARD_TYPE::ATTACK},
		//{3,CARD_TYPE::ATTACK},
		//{4,CARD_TYPE::ATTACK},
		//{5,CARD_TYPE::ATTACK},
		//{6,CARD_TYPE::ATTACK},
		//{7,CARD_TYPE::ATTACK},
		//{8,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK}




		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},
		//{9,CARD_TYPE::ATTACK},






	};



	const std::shared_ptr<CardDeck> GetHand(void)const { return deck_; }

	Enemy(void);
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
	void MoveDirFromInput(void)override;

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
	//カプセル関連
	static constexpr VECTOR CAP_LOCAL_TOP = { 0.0f, 150.0f*MODEL_SIZE_MULTIPLITER, 0.0f };	//トップ座標
	static constexpr VECTOR CAP_LOCAL_DOWN = { 0.0f,0.0f,0.0f };	//ダウン座標
	static constexpr float CAP_RADIUS = 30.0f* MODEL_SIZE_MULTIPLITER;				//カプセル球の半径


	Vector2 cardCenterPos_; //カードの中心座標

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

	//メンバ関数
	


	//デバッグ用で
};

