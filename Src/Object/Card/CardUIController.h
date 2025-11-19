#pragma once
#include <memory>
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include"./CardBase.h"
#include"../Application.h"

class CardUIDraw;

class CardUIController
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

	struct CARD_STATUS_DRAW
	{
		int typeImg = -1;
		CardBase::CARD_STATUS status;
	};

	//カードサイズ補完時間
	static constexpr float SCL_LERP_TIME = 0.5;
	//リロード1枚あたりの時間
	static constexpr float RELOAD_MOVE_CARD_TIME_PER = 0.06f;
	//カードセレクト時間
	static constexpr float SELECT_MOVE_CARD_TIME = 0.09f;
	//カード決定UI時間
	static constexpr float DISITION_MOVE_CARD_TIME = SELECT_MOVE_CARD_TIME;
	//敵の選択カード初期位置
	static constexpr Vector2F ENEMY_SELECT_CARD_START_POS = { Application::SCREEN_SIZE_X - 200,Application::SCREEN_HALF_Y + 200 };
	//弾かれるUI時間
	static constexpr float REACT_MOVE_CARD_TIME = 0.3f;


	// コンストラクタ
	CardUIController(int& _cardNumImgs);
	// デストラクタ
	~CardUIController(void);

	void Init(void);
	void Update(void);


	void Draw(void);
	//選択移動
	void DecisionMove(void);
	//弾かれ処理
	void ReactMove(const Vector2F& _goalPos);
	//使用済みカード削除
	void EraseUsedCard(void);

	/// @brief カード中心座標の取得
	/// @param  
	/// @return 中心座標
	Vector2F& GetCenterPos(void) { return cardPos_; }

	/// @brief カード大きさの取得
	/// @param  
	/// @return 
	float& GetScl(void) { return cardScl_; }

	/// @brief 現在角度を取得
	/// @param  
	/// @return 
	const float& GetCurrentAngle(void)const { return currentAngle_; }
	
	/// @brief カード座標初期化
	/// @param _num 何番目か
	/// @param _perRad 1枚の角度
	/// @param _center 楕円の中心座標
	/// @param _radius 半径
	void InitCard(const int& _num);

	/// @brief 使い済みにする
	/// @param  
	void ChangeUsedCard(void);

	//弾かれ状態
	void ChangeReactCard(void);

	/// @brief 使用中
	/// @param  
	void ChangeUsing(void);

	//弾かれ処理
	void ReactUpdate(const Vector2F& _goalPos);

	/// @brief リボルバー上を移動
	/// @param  
	void MoveOnRevolver(const float& _cnt,const float& moveTimeMax);

	/// @brief 敵のカード決定
	/// @param  
	void ChangeDicisionEnemyCardMove(void);

	/// @brief 拡大縮小カウントの取得
	/// @param  
	/// @return 拡大縮小カウント
	const float& GetSclCnt(void) { return sclCnt_; }

	/// @brief 決定した時のアニメーションカウントの取得
	/// @param  
	/// @return 
	const float& GetDecisionCnt(void)const { return disitionCnt_; }

	/// @brief ステータスの取得
	/// @param  
	/// @return ステータス
	const CardBase::CARD_STATUS GetStatus(void)const { return status_; }

	/// @brief 目標の角度のセット
	/// @param _rad 
	void SetGoalAngle(const float& _rad) { goalAngle_ = _rad; }

	/// @brief 現在の角度セット
	/// @param _rad 
	void SetCurrentAngle(const float& _rad) { currentAngle_ = _rad; }

	/// @brief 決定カウントセット
	/// @param _cnt 
	void SetDecisionCount(const float& _cnt) { disitionCnt_ = _cnt; }

	/// @brief ステータスの取得
	/// @param _status ステータス
	void SetStatus(const CardBase::CARD_STATUS& _status) { status_ = _status; }
	void SetTypeImg_(const int& _type) { typeImg_ = _type; }

	/// @brief リセット
	/// @param  
	void ResetCount(void);

	/// @brief 状態の取得
	/// @param  
	/// @return 
	const CARD_STATE& GetState(void)const { return state_; }










#ifdef _DEBUG
	/// @brief 弾かれカウントの取得
	const float& GetReactCount(void)const { return reactCnt_; }
#endif // _DEBUG


private:
	//決定したカードの座標
	static constexpr Vector2F DISITON_CARD_POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 200 };
	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_START_CARD_POS = { Application::SCREEN_HALF_X - 100.0f, Application::SCREEN_HALF_Y + 200.0f };

	//倍率1の時のカードの強さの描画座標
	static constexpr Vector2F NUM_LOCAL_POS = { 35.0f,53.0f };


	//楕円中心
	//static constexpr float CENTER_X = 80.0f;
	//static constexpr float CENTER_Y = 640.0f;
	///*static constexpr float CENTER_X = 0.0f;
	static constexpr float CENTER_X = 200.0f;
	static constexpr float CENTER_Y = 440.0f;
	//楕円の半径
	static constexpr float RADIUS_X = 186.0f;	//横半径
	//static constexpr float RADIUS_X = 170.0f;	//横半径
	static constexpr float RADIUS_Y = 214.0f;	//横半径
	static constexpr int ARROUND_NUM = 16;			//一周当たりの枚数		
	static constexpr int ARROUND_NUM_PER_QUAD = ARROUND_NUM / 4;//90度当たりの枚数
	static constexpr float ARROUND_PER_DEG = 360.0f / ARROUND_NUM;	//１枚当たりの角度
	static constexpr float ARROUND_PER_RAD = ARROUND_PER_DEG * DX_PI_F / 180.0f;//ラジアン変換

	//カード描画
	std::unique_ptr<CardUIDraw>cardDraw_;

	Vector2F cardPos_;		//カードの座標(画面外で初期化)
	Vector2F numPos_;		//カードの強さ番号座標(画面外で初期化)
	float currentAngle_;																		//カードの現在の角度
	float goalAngle_;	//カードの目標の角度
	float cardScl_;
	float sclCnt_;
	float disitionCnt_;													//決定カウント
	float reactCnt_;													//はじかれるカウント
	CARD_STATE state_;							//カードの状態

	Vector2F centerPos_;						//カードリボルバーの中心

	int typeImg_;		//カードの種類画像
	CardBase::CARD_STATUS status_;	//カードのステータス
	Vector2F size_;		//カードの大きさ
	int cardImg_;		//カード画像
	//カード番号イメージ
	int cardNoImg_;

	//カード画像作成
	int MakeCardUIImg(void);
};

