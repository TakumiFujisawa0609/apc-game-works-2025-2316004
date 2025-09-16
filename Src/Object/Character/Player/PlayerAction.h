#pragma once
#include<DxLib.h>
#include<memory>
#include<functional>
#include<map>
#include"../Manager/Generic/InputManager.h"
#include"../Object/Common/Transform.h"

class Player;
class PlayerInput;
class AnimationController;
class SceneManager;
class CardDeck;
class CardBase;
class ActionBase;
class InputBase;
class Idle;
class Run;
class Jump;
class PlayerAction
{

public:

	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 0.5f;
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	//格納するカードの強さ
	static constexpr int CARD_POWS[20] = { 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9 };
	
	//プレイヤーの入力される種類
	enum class ACTION_TYPE
	{
		NONE,	//何もなし
		INPUT,	//入力
		MOVE,	//移動
		DASHMOVE,//ダッシュ
		KNOCKBACK,//パンチされた状態
		JUMP,
		CARD_ACTION
	};

	enum class ACT_SE
	{
		DASH,	//ダッシュ
		JUMP,	//ジャンプ
		PUNCH,	//パンチ
		PUNCH_HIT,//パンチヒット
		SLIME,	//スライム
	};

	PlayerAction(InputBase& _input,Transform& _trans,CardDeck& _deck, InputManager::JOYPAD_NO _padNum);
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
	void ChangeAction(ACTION_TYPE _act);

	//移動量
	const VECTOR GetMovePow(void);

	//プレイヤーの角度Y
	const Quaternion GetPlayerRotY(void) { return playerRotY_; }

	//状態
	const ACTION_TYPE GetAct(void)const { return act_; }

	//移動量
	void SetMovePow(const VECTOR _movePow) { movePow_ = _movePow; }

	//スピード
	void SetSpeed(const float _spd) { speed_ = _spd; }

	//方向
	void SetDir(const VECTOR _dir) { dir_ = _dir; }

	//再生しているリソースをすべて止める
	void StopResource(void);

#ifdef _DEBUG
	static constexpr float CARD_ACT_TIME_MAX = 5.0f; //カードアクション時間(デバッグ用)
#endif // _DEBUG


private:
	//プレイヤーナンバー(カードデッキで判定する用)
	static constexpr int PLAYER_NUM = 0;
	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;
	//移動
	static constexpr float MOVE_SPEED = 6.0f;	//移動スピード
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

	// シーンマネージャ参照
	SceneManager& scnMng_;
	//アニメーションコントローラー
	//AnimationController& animationController_;
	//状態遷移
	std::map<ACTION_TYPE, std::function<void(void)>>changeAction_;
	//状態更新
	std::function<void(void)>actionUpdate_;

	//状態
	ACTION_TYPE act_;
	bool isCardAct_;	//カードアクション中かどうか
	float cardActTime_; //カードアクション時間(デバッグ用)
	InputManager::JOYPAD_NO padNum_;		//パッド番号
	Transform& trans_;	//モデル情報
	InputBase& input_;	//各キャラクターの入力情報
	//素材関連
	//------------------------


	//アクション関連
	//------------------------
	std::unique_ptr<ActionBase>action_;
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>actionObject_;

	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR dir_;			//方向

	//回転
	Quaternion playerRotY_;		//プレイヤーY角度
	Quaternion goalQuaRot_;		//目的の回転
	float stepRotTime_;			//補完時間


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

	//何もなし
	void ChangeNone(void);

	//更新
	void MoveUpdate(void);		//移動
	void JumpUpdate(void);		//ジャンプ
	void CardUseUpdate(void);	//カード使用
	void CardChargeUpdate(void);//ストック

	//状態遷移
	void ChangeJump(void);		//ジャンプ
	void ChangeInput(void);		//入力
	void ChangeDashMove(void);	//ダッシュ
	void ChangeCardUse(void);	//カード使用


	//移動状態変更
	void ChangeMove(void);

	//ダッシュ

	//毎フレーム移動方向とスピードを更新する
	void UpdateMoveDirAndPow(void);


	////ジャンプ
	//void Jump(void);

	//カード関連
	//カード使用
	//カードチャージ
	//カード選択
	void CardMove(void);

	//ジャンプができる条件
	bool CheckJumpInput(void);

	//カードデッキ
	CardDeck& deck_;

	/// <summary>
	/// 再生させたいSE以外すべて止める
	/// </summary>
	/// <param name="_se">現在再生させたいSE</param>
	//void StopSe(const ACT_SE _se);
};

