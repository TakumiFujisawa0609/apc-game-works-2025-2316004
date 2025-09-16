#pragma once
#include <map>
#include <functional>
#include <memory>
#include"../../Character/Player/InputController.h"
#include"./PlayerAction.h"
#include "../../ObjectBase.h"

#define DEBUG_ON
class AnimationController;
class Camera;
class PlayerAction;
class PlayerOnHit;
class DateBank;
class Shadow;
class Camera;
class CardDeck;
class ModelMaterial;
class ModelRenderer;

class EffectController;

class Player :public ObjectBase
{
public:
	//******************************************
	//定数
	//******************************************
	//半径
	static constexpr float RADIUS = 25.0f;
	//デフォルトのアニメーションスピード
	static constexpr float DEFAULT_ANIM_SPD = 60.0f;

	//格納するカードの強さ
	static constexpr int CARD_POWS[20] = { 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9 };
	//******************************************


	enum class FLOOR_COL
	{
		NONE
		,SIDE
		,TOP
	};

	enum class ATK_ACT
	{
		NONE,	//何もなし
		INPUT,	//入力
		MOVE,	//移動
		PUNCH,	//パンチ
		KNOCKBACK,//パンチされた状態
		JUMP
	};

	enum class PLAYER_STATE
	{
		ALIVE
		,DEATH
		,GOAL
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE = 0,
		IDLE = 1,
		WALK = 2,
		FALL = 4,
		HAND_UP = 5,
		HAND_WAVE = 6,
		DAMAGE = 9,
		PUNCH = 12,
		JUMP = 13,
		LAND=14,
		GOAL=6,
	};

	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <param name="_cntl">コントローラー識別番号</param>
	/// <param name="_tag">プレイヤーごとのタグ</param>
	Player(void);

	// デストラクタ
	~Player(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void)override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;

	///// <summary>
	///// 当たり判定後の処理
	///// </summary>
	///// <param name="_hitColTag">相手側の当たり判定</param>
	//void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//ゲッタ
	//******************************************
	//プレイヤー番号
	inline const int GetPlayerNum(void)const { return playerNum_; }

	//使用するコントローラー
	inline const InputManager::CONTROLL_TYPE GetCntl(void)const { return cntl_; }

	//コントローラー番号
	inline const InputManager::JOYPAD_NO GetPadNum(void)const { return padNum_; }

	//プレイヤーの手札
	//const std::weak_ptr<CardDeck> GetHand(void)const { return action_->GetHand(); }

	//手札
	const std::weak_ptr<CardDeck> GetHand(void) { return deck_; }

	//******************************************
	//セッタ
	//******************************************
	/// <summary>
	/// 座標
	/// </summary>
	/// <param name="_worldPos">ワールド座標</param>
	//void SetPos(const VECTOR _worldPos);


	//*****************************************




private:
	//***********************************************
	//定数
	//***********************************************
	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;
	//プレイヤー１のX座標
	static constexpr float PLAYER_ONE_POS_X = -20.0f;
	//座標の間隔
	static constexpr float DISTANCE_POS = 50.0f;
	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;
	//落ちるアニメーションのスタート
	static constexpr float FALL_ANIM_START = 32.0f;
	//落ちるアニメーションの終わり
	static constexpr float FALL_ANIM_END = 59.0f;
	//死ぬ判定の座標の基準
	static constexpr float DEATH_POS_Y = -600.0f;
	//死んだときのパッド振動時間
	static constexpr int DEATH_PAD_VIBRATION_TIME = 300;
	//死んだときのパッド振動の強さ
	static constexpr int DEATH_PAD_VIBRATION_POW = 300;
	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };
	//パンチの範囲
	static constexpr float PUNCH_RADIUS = 50.0f;
	//プレイヤーナンバー(デッキクラスで判定用)
	static constexpr int PLAYER_NUM = 0;
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;

	//--------------------------------------------------
	//当たり判定
	//--------------------------------------------------
	//ラインの長さ
	static constexpr float LINE_RANGE = 10.0f;
	//プレイヤーの上の座標
	static constexpr VECTOR LOCAL_UP_POS = { 0.0f,RADIUS+ LINE_RANGE,0.0f };
	//プレイヤーの下
	static constexpr VECTOR LOCAL_DOWN_POS = { 0.0f,-RADIUS- LINE_RANGE,0.0f };
	//--------------------------------------------------
	//ゲームオーバーの待機時間
	static constexpr float DEATH_DELAY = 2.0f;

	//ゴール時の待機時間
	static constexpr float GOAL_DELAY = 1.0f;

	//***********************************
	//アニメーション関連
	//***********************************

	//当たり判定を行う範囲
	static constexpr int COL_RANGE = 1;

	//入力デバイス
	InputManager::CONTROLL_TYPE cntl_;
	//ゲームパッド番号
	InputManager::JOYPAD_NO padNum_;
	//入力
	std::unique_ptr<InputController>input_;
	//行動系
	std::unique_ptr<PlayerAction>action_;
	// アニメーション
	std::unique_ptr<AnimationController> animationController_;
	//カメラ
	std::weak_ptr<Camera>camera_;

	std::shared_ptr<CardDeck>deck_;

	//メンバ変数
	//--------------------------------------------
	//プレイヤー単体が持っているもの
	int playerNum_;			//プレイヤー番号
	//計測用
	float time_;
	//ゴール時間格納
	float goalTime_;
	//プレイヤー状態
	PLAYER_STATE state_;	//プレイヤーの状態(生存状態)
	//プレイヤーの状態遷移
	std::map<PLAYER_STATE, std::function<void(void)>>changeStates_;
	//状態更新
	std::function<void(void)>stateUpdate_;

	float finishDelay_;	//ゲーム終了時の待機時間
	Vector2 cardCenterPos_;	//カードの位置
	//プレイヤーナンバー(カードデッキで判定する用)


	//--------------------------------------------
	//******************************************
	//メンバ関数
	//******************************************
#ifdef DEBUG_ON
	void DrawDebug(void);
#endif // DEBUG_ON
	//プレイヤー状態
	//**************************************************
	//状態遷移
	void ChangeState(PLAYER_STATE _state);
	//生存しているとき
	//------------------------------
	//状態遷移
	void ChangeAlive(void);
	//生存更新
	void AliveUpdate(void);
	//------------------------------
	//死亡しているとき
	//------------------------------
	void ChangeDeath(void);
	void DeathUpdate(void);
	//------------------------------
	//ゴールした時
	void ChangeGoal(void);
	void GoalUpdate(void);
	
	//アクション関係
	void Action(void);


};

