#pragma once
#include<DxLib.h>
#include<memory>
#include<functional>
#include<map>

class Player;
class PlayerInput;
class AnimationController;
class CardDeck;
class CardBase;
class PlayerAction
{
public:
	//定数
	//****************************************
	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 0.5f;
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	//格納するカードの強さ
	static constexpr int CARD_POWS[20] = { 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9 };
	//static constexpr int CARD_POWS[20] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};
	//****************************************

	enum class ATK_ACT
	{
		NONE,	//何もなし
		INPUT,	//入力
		MOVE,	//移動
		DASHMOVE,//ダッシュ
		PUNCH,	//パンチ
		KNOCKBACK,//パンチされた状態
		JUMP
	};

	enum class ACT_SE
	{
		DASH,	//ダッシュ
		JUMP,	//ジャンプ
		PUNCH,	//パンチ
		PUNCH_HIT,//パンチヒット
		SLIME,	//スライム
	};

	PlayerAction(Player& _player, SceneManager& _scnMng, AnimationController& _animationController);
	~PlayerAction(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);


	void DrawDebug(void);



	//状態遷移
	void ChangeAction(ATK_ACT _act);



	//*****************************************
	//ゲッタ
	//*****************************************
	//移動量
	const VECTOR GetMovePow(void) { return movePow_; }

	//ジャンプ力
	const VECTOR GetJumpPow(void) { return jumpPow_; }

	//プレイヤーの角度Y
	const Quaternion GetPlayerRotY(void) { return playerRotY_; }

	//手札
	const std::weak_ptr<CardDeck> GetHand(void) { return deck_;}

	//ジャンプ中
	const bool GetIsJump(void) { return isJump_; }

	//状態
	const ATK_ACT GetAct(void)const { return act_; }

	//*****************************************
	//セッタ
	//*****************************************
	//ジャンプ力
	void SetJumpPow(const VECTOR _jumpPow) { jumpPow_ = _jumpPow; };

	//移動量
	void SetMovePow(const VECTOR _movePow) { movePow_ = _movePow; }

	//スピード
	void SetSpeed(const float _spd) { speed_ = _spd; }

	//空中かどうか
	void SetIsJump(const bool _isJump) { isJump_ = _isJump; }

	//ジャンプ減速
	void SetJumpDecel(const float _decel) { jumpDeceralation_ = _decel; }

	//ジャンプ時間
	void SetStepJump(const float _step) { stepJump_ = _step; }

	//方向
	void SetDir(const VECTOR _dir) { dir_ = _dir; }

	//再生しているリソースをすべて止める
	void StopResource(void);

	//デバッグ
	float GetJumpDecel(void) { return jumpDeceralation_; }
	float GetStepJump(void) { return stepJump_; }

private:
	//*******************************************
	//定数
	//*******************************************
	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;
	//移動
	//----------------------------------
	//移動スピード
	static constexpr float MOVE_SPEED = 6.0f;

	//スライム床上での移動速度(通常)
	static constexpr float SLIME_FLOOR_MOVE_SPD = 3.0f;

	//ダッシュスピード
	static constexpr float DASH_SPEED = 11.0f;
	//ぶっ飛ぶスピード
	static constexpr float FLY_AWAY_SPEED = 12.0f;
	//落ちているときの重力制限(jumpPowに加算しているのでjumpPowに適用)
	static constexpr float LIMIT_GRAVITY = -20.0f;

	//ダッシュアニメーションスピード
	static constexpr float DASH_ANIM_SPEED = 200.0f;

	//ダッシュSE間隔
	static constexpr float DASH_SE_TIME = 0.2f;

	//----------------------------------
	//ジャンプ
	//----------------------------------

	//ジャンプ加速の倍率
	static constexpr float TIME_JUMP_SCALE = 1.0f;
	//スライム床上でのジャンプ力
	static constexpr float SLIME_FLOOR_JUMP_POW = 10.0f;

	//ジャンプアニメーションループ開始
	static constexpr float JUMP_ANIM_LOOP_START_FRAME = 23.0f;
	//ジャンプアニメーションループ完了
	static constexpr float JUMP_ANIM_LOOP_END_FRAME = 25.0f;
	//ジャンプアニメーションループ中のスピード
	static constexpr float JUMP_ANIM_ATTACK_BLEND_TIME = 5.0f;

	//ジャンプ開始アニメステップ
	static constexpr float JUMP_ANIM_START_FRAME = 10.0f;
	static constexpr float JUMP_ANIM_END_FRAME = 60.0f;

	//ジャンプ力
	static constexpr float POW_JUMP = 20.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;

	//-------------------------------------------------
	//メンバ変数
	//-------------------------------------------------
	// シーンマネージャ参照
	SceneManager& scnMng_;

	//プレイヤー
	Player& player_;

	//アニメーションコントローラー
	AnimationController& animationController_;

	//状態遷移
	std::map<ATK_ACT, std::function<void(void)>>changeAction_;

	//状態更新
	std::function<void(void)>actionUpdate_;

	//操作入力
	std::unique_ptr<PlayerInput> input_;

	//状態
	ATK_ACT act_;

	//素材関連
	//------------------------


	//移動
	//------------------------
	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR dir_;			//方向

	//回転
	Quaternion playerRotY_;		//プレイヤーY角度
	Quaternion goalQuaRot_;		//目的の回転
	float stepRotTime_;			//補完時間



	//ジャンプ
	//-----------------------
	bool isJump_;			// ジャンプ判定
	float stepJump_;		// ジャンプの入力受付時間
	VECTOR jumpPow_;		// ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量

	//パンチ
	//-----------------------
	bool isPunchHitTime_;		//パンチ当たり判定の時間フラグ
	float punchCnt_;				//パンチカウント
	float punchCoolCnt_;			//パンチクールタイム
	VECTOR punchPos_;			//攻撃座標
	float punchedCnt_;			//パンチ効果時間カウント

	//山札(デッキクラスに格納用)
	std::vector<std::shared_ptr<CardBase>>drawPile_;
	Vector2 cardCenterPos_;	//カードの位置
	//-------------------------------------------------
	//メンバ関数
	//-------------------------------------------------
	//何もしない
	void NoneUpdate(void);

	//入力
	void ActionInputUpdate(void);
	void ChangeInput(void);

	//何もなし
	void ChangeNone(void);

	//移動状態の更新
	void MoveUpdate(void);
	//入力方向に応じて方向を決める
	void MoveDirFronInput(void);
	//移動状態変更
	void ChangeMove(void);

	//ダッシュ
	void ChangeDashMove(void);

	//毎フレーム移動方向とスピードを更新する
	void UpdateMoveDirAndPow(void);
	//移動速度
	void Speed(void);

	//ジャンプ
	void JumpUpdate(void);
	void Jump(void);
	void ChangeJump(void);

	//カード関連
	//カード使用
	void CardUseUpdate(void);
	//カードチャージ
	void CardChargeUpdate(void);
	//カード選択
	void CardMove(void);

	//ジャンプができる条件
	bool CheckJumpInput(void);

	//カードデッキ
	std::shared_ptr<CardDeck>deck_;

	/// <summary>
	/// 再生させたいSE以外すべて止める
	/// </summary>
	/// <param name="_se">現在再生させたいSE</param>
	//void StopSe(const ACT_SE _se);

	//回転
	void Rotate(void);
	//最終的に動かしたい角度の設定
	void SetGoalRotate(double _deg);


};

