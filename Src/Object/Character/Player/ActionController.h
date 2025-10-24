#pragma once
#include<DxLib.h>
#include<memory>
#include<functional>
#include<map>
#include"../Manager/Generic/InputManager.h"
#include"../Object/Common/Transform.h"

class CharacterBase;
class Player;
class PlayerInput;
class AnimationController;
class SceneManager;
class CardDeck;
class CardBase;
class ActionBase;
class LogicBase;
class Idle;
class Run;
class Jump;
class React;
class PlayerCardAction;

class ActionController
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
		IDLE,		//何もしてない
		MOVE,		//移動
		DASHMOVE,	//ダッシュ
		REACT,	//パンチされた状態
		JUMP,		//ジャンプ
		CARD_ACTION	//カードアクション
	};



	enum class ACT_SE
	{
		DASH,	//ダッシュ
		JUMP,	//ジャンプ
		PUNCH,	//パンチ
		PUNCH_HIT,//パンチヒット
		SLIME,	//スライム
	};

	ActionController(CharacterBase& _charaObj,LogicBase& _input, Transform& _trans, CardDeck& _deck, AnimationController& _anim, InputManager::JOYPAD_NO _padNum);
	~ActionController(void);

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

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_act">遷移したいアクション</param>
	void ChangeAction(ACTION_TYPE _act);

	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動量</returns>
	const VECTOR GetMovePow(void){ return movePow_; }

	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>状態</returns>
	const ACTION_TYPE GetAct(void)const { return act_; }

	/// <summary>
	/// 入力クラスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>入力クラス</returns>
	LogicBase& GetInput(void) { return logic_; }

	/// <summary>
	/// アニメーションの再生
	/// </summary>
	/// <param name="_animType"></param>
	AnimationController& GetAnimation(void) { return anim_; }

	/// <summary>
	/// 攻撃当たり判定が生きているか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool GetIsAtkColAlive(void);
	

	/// <summary>
	/// 角度Yを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Quaternion GetPlayerRotY(void);
	/// <summary>
	/// 再生しているリソースをすべて止める
	/// </summary>
	/// <param name=""></param>
	void StopResource(void);

	/// @brief カード選択条件
	/// @param  
	/// @return true:カード選択できる
	const bool IsCardDisitionControll(void);


	template <typename T, typename... Args>
	/// @brief メインアクションの追加
	/// @param _action 
	//void AddMainAction(const ACTION_TYPE _type, Args&&... args);
	void AddMainAction(const ACTION_TYPE _type, Args && ...args)
	{
		mainAction_[_type] = std::make_unique<T>(std::forward<Args>(args)...);
	}


#ifdef _DEBUG
	//デバッグb
	void DrawDebug(void);
	//カードアクション時間(デバッグ用)
	static constexpr float CARD_ACT_TIME_MAX = 5.0f; 
#endif // _DEBUG


private:
	//プレイヤーナンバー(カードデッキで判定する用)
	static constexpr int PLAYER_NUM = 0;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.0f;

	// シーンマネージャ参照
	SceneManager& scnMng_;
	//アニメーションコントローラー
	AnimationController& anim_;
	//状態遷移
	std::map<ACTION_TYPE, std::function<void(void)>>changeAction_;
	//状態更新
	std::function<void(void)>actionUpdate_;
	//カードデッキ
	CardDeck& deck_;
	//オブジェクト(当たり判定用)
	CharacterBase& charaObj_;
	//状態
	ACTION_TYPE act_;
	//パッド番号
	InputManager::JOYPAD_NO padNum_;		
	//カード関連
	bool isCardAct_;	//カードアクション中かどうか
	float cardActTime_; //カードアクション時間(デバッグ用)

	//モデル情報
	Transform& trans_;	
	//各キャラクターの入力情報
	LogicBase& logic_;	

	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量

	//回転
	Quaternion playerRotY_;		//プレイヤーY角度
	Quaternion goalQuaRot_;		//目的の回転
	float stepRotTime_;			//補完時間
	VECTOR dir_;			//方向




	//プレイヤーのメインとなるアクション(移動やジャンプなど)
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>mainAction_;
	//サブアクション(カードセレクトなど同時並行となるもの)
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>subAction_;

	//山札(デッキクラスに格納用)
	std::vector<std::shared_ptr<CardBase>>drawPile_;
	//カードの位置
	Vector2 cardCenterPos_;	

	//カード選択
	void CardMove(void);

	//カードチャージ
	void CardChargeUpdate(void);

	/// <summary>
	/// 再生させたいSE以外すべて止める
	/// </summary>
	/// <param name="_se">現在再生させたいSE</param>
	//void StopSe(const ACT_SE _se);

	//入力方向に応じて方向を決める
	void MoveDirFronInput(void);
	//回転処理
	void Rotate(void);
	//方向と移動量の更新
	void DirAndMovePowUpdate(void);



};


