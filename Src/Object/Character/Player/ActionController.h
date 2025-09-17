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
class CardAction;

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
		KNOCKBACK,	//パンチされた状態
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

	ActionController(InputBase& _input,Transform& _trans,CardDeck& _deck, InputManager::JOYPAD_NO _padNum);
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


	void DrawDebug(void);

	//状態遷移
	void ChangeAction(ACTION_TYPE _act);

	//移動量
	const VECTOR GetMovePow(void);
	//状態
	const ACTION_TYPE GetAct(void)const { return act_; }
	//入力クラスの取得
	InputBase& GetInput(void) { return input_; }

	//再生しているリソースをすべて止める
	void StopResource(void);

#ifdef _DEBUG
	static constexpr float CARD_ACT_TIME_MAX = 5.0f; //カードアクション時間(デバッグ用)
#endif // _DEBUG


private:
	//プレイヤーナンバー(カードデッキで判定する用)
	static constexpr int PLAYER_NUM = 0;


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


	//プレイヤーのメインとなるアクション(移動やジャンプなど)
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>mainAction_;
	//サブアクション(カードセレクトなど同時並行となるもの)
	std::map<ACTION_TYPE, std::unique_ptr<ActionBase>>subAction_;

	//山札(デッキクラスに格納用)
	std::vector<std::shared_ptr<CardBase>>drawPile_;
	Vector2 cardCenterPos_;	//カードの位置
	//-------------------------------------------------
	//メンバ関数
	//-------------------------------------------------
	void CardUseUpdate(void);	//カード使用
	void CardChargeUpdate(void);//ストック

	//状態遷移
	void ChangeCardUse(void);	//カード使用



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

