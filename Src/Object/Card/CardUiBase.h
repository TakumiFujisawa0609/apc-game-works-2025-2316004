#pragma once
#include <functional>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include <list>
#include "../ObjectBase.h"
#include "CardUiBase.h"
#include "./CardBase.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include"../Application.h"

class PixelMaterial;
class PixelRenderer;
class CardUIController;
class CardUIDraw;

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

	enum class CARD_NUMBER
	{
		CARD_ZERO,
		CARD_ONE,
		CARD_TWO,
		CARD_THREE,
		CARD_FOUR,
		CARD_FIVE,
		CARD_SIX,
		CARD_SEVEN,
		CARD_EIGHT,
		CARD_NINE

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
	virtual void Load(void);

	/// @brief 初期化
	/// @param  
	virtual void Init(void);

	/// @brief 更新
	/// @param  
	virtual void Update(void);

	/// @brief 描画
	/// @param
	virtual void Draw(void);




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

	/// @brief アクションカード配列の状態を使用中にする
	/// @param  
	void ChangeUsingActionCard(void);



	/// @brief リロードカウントセット
	/// @param _cnt 
	void SetReloadCount(const float _cnt) { reloadPer_ = _cnt; };

	/// @brief カードリボルバーの状態の取得
	/// @param  
	/// @return カードリボルバーの状態
	const CARD_SELECT GetSelectState(void)const { return selectState_; }

	/// @brief カード初期化
	/// @param  
	virtual void InitCardUI(void) = 0;

protected:

	//カードUI状態
	
	//決定したカードの座標
	static constexpr Vector2F DISITON_CARD_POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 200 };
	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_START_CARD_POS = { Application::SCREEN_HALF_X-100.0f, Application::SCREEN_HALF_Y + 200.0f };
	//デルタ
	static constexpr float DELTA = 1.0f/60.0f;
	//カード数字による画像名
	static constexpr std::wstring_view CARD_ZERO_NAME = L"Zero";
	static constexpr std::wstring_view CARD_ONE_NAME = L"One";
	static constexpr std::wstring_view CARD_TWO_NAME = L"Two";
	static constexpr std::wstring_view CARD_THREE_NAME = L"Three";
	static constexpr std::wstring_view CARD_FOUR_NAME = L"Four";
	static constexpr std::wstring_view CARD_FIVE_NAME = L"Five";
	static constexpr std::wstring_view CARD_SIX_NAME = L"Six";
	static constexpr std::wstring_view CARD_SEVEN_NAME = L"Seven";
	static constexpr std::wstring_view CARD_EIGHT_NAME = L"Eight";
	static constexpr std::wstring_view CARD_NINE_NAME = L"Nine";



	//カード更新関数
	std::function<void(void)>cardUpdate_;
	//状態遷移
	std::unordered_map<CARD_SELECT, std::function<void(void)>>changeMoveState_;
	//タイプ画像
	std::unordered_map<CardBase::CARD_TYPE, int> typeImgs_;
	//①初期カード
	std::vector<std::shared_ptr<CardUIController>>initialCards_;
	//②手札
	std::list<std::shared_ptr<CardUIController>>handCards_;
	//③手札UIは派生のPlayerCardUI
	//④手札の現在選択中カード
	std::list<std::shared_ptr<CardUIController>>::iterator handCurrent_;
	//⑤アクション中カード
	std::list<std::shared_ptr<CardUIController>>actions_;
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

	//UI描画更新
	virtual void UpdateDrawCardUI(void) = 0;;

	//アクション配列のカードをすべて決定移動させる
	void DecisionMoveCardAll(void);
	//使用済みのカードを消す
	void UpdateUsedCard(void);
	// カード弾かれ移動
	void ReactMoveCard(const Vector2F& _goalPos);
	//特定のカードを弾かれ移動させる
	void ReactMoveSpecificCard(CARD_UI_INFO& _card, const Vector2F& _goalPos);

	//属性画像取得
	const int GetTypeImg(const CardBase::CARD_STATUS _status);


	//カードUI画像作成
	int MakeCardUIImg(void);
};

