#pragma once
#include <map>
#include <functional>
#include <memory>
#include<string>
//#include"../../Character/Player/InputController.h"
#include"./ActionController.h"
#include "../Base/CharacterBase.h"

#define DEBUG_ON
class AnimationController;
class ActionController;
class CardDeck;
class InputController;
class DateBank;
class Camera;
class Shadow;
class Camera;
class ModelMaterial;
class ModelRenderer;

class EffectController;

class Player :public CharacterBase
{
public:

	//半径
	static constexpr float RADIUS = 25.0f;
	//デフォルトのアニメーションスピード
	static constexpr float DEFAULT_ANIM_SPD = 60.0f;

	//格納するカードの強さ
	//static constexpr int CARD_POWS[20] = { 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9 };
	static constexpr int CARD_POWS[20] = { 0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9 };


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
		REACT,//パンチされた状態
		JUMP
	};

	enum class PLAYER_STATE
	{
		ALIVE
		,DEATH
		,GOAL
	};

	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <param name="_cntl">コントローラー識別番号</param>
	/// <param name="_tag">プレイヤーごとのタグ</param>
	Player(void);

	// デストラクタ
	~Player(void)override;

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

	/// <summary>
	/// プレイヤー番号
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー番号</returns>
	inline const int GetPlayerNum(void)const { return playerNum_; }

	/// <summary>
	/// 使用するコントローラー
	/// </summary>
	/// <param name=""></param>
	/// <returns>使用するコントローラー</returns>
	inline const InputManager::CONTROLL_TYPE GetCntl(void)const { return cntl_; }

	/// <summary>
	/// コントローラー番号
	/// </summary>
	/// <param name=""></param>
	/// <returns>コントローラー番号</returns>
	inline const InputManager::JOYPAD_NO GetPadNum(void)const { return padNum_; }

	//プレイヤーの手札
	//const std::weak_ptr<CardDeck> GetHand(void)const { return action_->GetHand(); }

	/// <summary>
	/// 手札
	/// </summary>
	/// <param name=""></param>
	/// <returns>手札</returns>
	const std::weak_ptr<CardDeck> GetHand(void) { return deck_; }




private:

	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;
	//プレイヤー１のX座標
	static constexpr float PLAYER_ONE_POS_X = -20.0f;
	//座標の間隔
	static constexpr float DISTANCE_POS = 50.0f;
	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;
	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };
	//パンチの範囲
	static constexpr float PUNCH_RADIUS = 50.0f;
	//プレイヤーナンバー(デッキクラスで判定用)
	static constexpr int PLAYER_NUM = 0;
	//カード最大枚数
	static constexpr int CARD_NUM_MAX = 20;
	//プレイヤーの腰のフレーム番号
	static constexpr int SPINE_FRAME_NO = 0;


	//***********************************
	//アニメーション関連
	//***********************************

	//当たり判定を行う範囲
	static constexpr int COL_RANGE = 1;

	//入力デバイス
	InputManager::CONTROLL_TYPE cntl_;
	//ゲームパッド番号
	InputManager::JOYPAD_NO padNum_;
	//カメラ
	std::weak_ptr<Camera>camera_;



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

#ifdef DEBUG_ON
	void DrawDebug(void);
#endif // DEBUG_ON
	//プレイヤー状態
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

	//Jsonでプレイヤーの座標を保存してみる
	void SavePos(const std::wstring _fileName);

	void LoadPos(const std::wstring _fileName);
};

