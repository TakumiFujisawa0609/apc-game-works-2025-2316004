#pragma once
#include<functional>
#include<map>
#include<memory>
#include<vector>
#include<list>
#include"CardUiBase.h"
#include"./CardBase.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include"../Application.h"

class PixelMaterial;
class PixelRenderer;
class CardUIController;
class CardUI;

class CardUIBase
{
public:
	enum class CARD_STATE
	{
		DRAW_PILE		//山札
		, MOVE_DRAW		//山札から手札へ移動
		, USING			//使用中
		, REACT			//弾かれ中
		, USED			//使用済み
	};

	enum class CARD_SELECT
	{
		NONE
		, LEFT
		, RIGHT
		, DISITION
		, RELOAD_WAIT
		, RELOAD

	};
	//カードサイズ補完時間
	static constexpr double SCL_LERP_TIME = 0.5;

	struct CARD_UI_INFO
	{
		CardBase::CARD_STATUS status;										//カードのステータス
		int typeImg = -1;																				//カードの種類画像
		Vector2F cardPos_ = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの座標(画面外で初期化)
		Vector2F numPos_ = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの強さ番号座標(画面外で初期化)
		float currentAngle_ = 0.0f;																		//カードの現在の角度
		float goalAngle_ = currentAngle_;	//カードの目標の角度
		double cardScl_ = 1.0;
		double sclCnt = SCL_LERP_TIME;
		float disitionCnt_;													//決定カウント
		float reactCnt_;													//はじかれるカウント
		CARD_STATE state_ = CARD_STATE::DRAW_PILE;							//カードの状態
	};
	// コンストラクタ
	CardUIBase(void);

	// デストラクタ
	virtual ~CardUIBase(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void) = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) = 0;

	/// @brief 描画
	/// @param
	virtual void Draw(void) = 0;




	/// @brief UI配列に追加
	/// @param _status 
	void AddCardUi(const CardBase::CARD_STATUS _status);

	/// @brief カード状態選択
	/// @param _select 
	void ChangeSelectState(const CARD_SELECT _select);

	/// @brief カードの決定
	/// @param  
	void CardDecision(void);

	/// @brief アクションカード配列の状態を使用済みにする
	/// @param  
	void ChangeUsedActionCard(void);

	/// @brief アクションカード配列の状態を弾かれ中にする(カード負けしたとき)
	/// @param  
	void ChangeReactActionCard(void);



	/// @brief リロードカウントセット
	/// @param _cnt 
	void SetReloadCount(const float _cnt) { reloadPer_ = _cnt; };

	/// @brief カードリボルバーの状態の取得
	/// @param  
	/// @return カードリボルバーの状態
	const CARD_SELECT GetSelectState(void)const { return selectState_; }



protected:
	//倍率1の時のカードの強さの描画座標
	static constexpr Vector2F NUM_LOCAL_POS = { 35.0f,53.0f };
	//リロード1枚あたりの時間
	static constexpr float RELOAD_MOVE_CARD_TIME_PER = 0.07f;
	//カードセレクト時間
	static constexpr float SELECT_MOVE_CARD_TIME = 0.5f;
	//カード決定UI時間
	static constexpr float DISITION_MOVE_CARD_TIME = SELECT_MOVE_CARD_TIME;
	//static constexpr float DISITION_MOVE_CARD_TIME = 0.7;
	//弾かれるUI時間
	static constexpr float REACT_MOVE_CARD_TIME = 0.9f;

	//カードUI状態
	
	//決定したカードの座標
	static constexpr Vector2F DISITON_CARD_POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 200 };
	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_START_CARD_POS = { Application::SCREEN_HALF_X-100.0f, Application::SCREEN_HALF_Y + 200.0f };

	//デルタ
	static constexpr float DELTA = 1.0f/60.0f;

	//カード更新関数
	std::function<void(void)>cardUpdate_;
	//状態遷移
	std::map<CARD_SELECT, std::function<void(void)>>changeMoveState_;


	//シェーダー関連
	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>renderer_;

	//円形UIの中心座標
	Vector2 centerPos_;
	//カード番号イメージ
	int* cardNoImg_;
	//攻撃カード画像
	int atkCardImg_;
	//リロードカード画像
	int reloadCardImg_;
	//カードナンバー座標
	Vector2F numPos_;
	//カードセレクトの動き時間
	float cardMoveCnt_;
	//決定後のカウント
	float disitionCnt_;
	//リロード割合(カードのゲージ計算用)
	float reloadPer_;

	//状態
	CARD_SELECT selectState_;
	//アクション中カード
	std::list<std::shared_ptr<CardUIController>>actions_;
	//初期カード
	std::vector<std::shared_ptr<CardUIController>>uiInfos_;
	//手札
	std::list<std::shared_ptr<CardUIController>>handCards_;
	//手札の現在選択中カード
	std::list<std::shared_ptr<CardUIController>>::iterator handCurrent_;

	//手札選択カードの計算
	void AddHandCurrent(void);	//足し算
	void SubHandCurrent(void);	//引き算

	//カード状態遷移
	virtual void ChangeNone(void) {};		//通常
	virtual void ChangeLeft(void) {};		//左に移動
	virtual void ChangeRight(void) {};		//右に移動
	virtual void ChangeDecision(void) {};	//決定
	virtual void ChangeReloadWait(void) {};

	virtual void UpdateNone(void) {};
	virtual void UpdateLeft(void) {};
	virtual void UpdateRight(void) {};
	virtual void UpdateDecision(void) {};
	virtual void UpdateReloadWait(void) {};

	//カード初期化
	virtual void InitCardUI(void) = 0;;

	//アクション配列のカードをすべて決定移動させる
	void DecisionMoveCardAll(void);
	//アクション配列の特定のカードを決定移動させる
	void DecisionMoveSpecificCard(CARD_UI_INFO& _card);
	//使用済みのカードを消す
	void UpdateUsedCard(void);
	// カード弾かれ移動
	void ReactMoveCard(const Vector2F& _goalPos);
	//特定のカードを弾かれ移動させる
	void ReactMoveSpecificCard(CARD_UI_INFO& _card, const Vector2F& _goalPos);
	//カード描画
	void DrawCard(const CARD_UI_INFO _card);

	//カードUI画像作成
	int MakeCardUIImg(void);
};

