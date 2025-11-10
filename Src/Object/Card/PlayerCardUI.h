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
class PlayerCardUI
	:public CardUIBase
{

public:
	//enum class CARD_STATE
	//{
	//	DRAW_PILE
	//	, MOVE_DRAW
	//	, USING
	//	, REACT
	//	, USED
	//};

	//enum class CARD_SELECT
	//{
	//	NONE
	//	, LEFT
	//	, RIGHT
	//	, DISITION
	//	, RELOAD
	//};
	////カードサイズ補完時間
	//static constexpr double SCL_LERP_TIME = 0.5;
	//struct CARD_UI_INFO
	//{
	//	Vector2F cardPos = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの座標(画面外で初期化)
	//	Vector2F numPos = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの強さ番号座標(画面外で初期化)
	//	int typeImg = -1;																				//カードの種類画像
	//	float currentAngle = 0.0f;																		//カードの現在の角度
	//	float goalAngle = currentAngle;	//カードの目標の角度
	//	double cardScl = 1.0;
	//	double sclCnt = SCL_LERP_TIME;
	//	CardBase::CARD_STATUS status;																	//カードのステータス
	//	CARD_STATE state = CARD_STATE::DRAW_PILE;														//カードの状態
	//};

	/// @brief コンストラクタ
	/// @param  
	PlayerCardUI(void);

	//デストラクタ
	~PlayerCardUI(void)override;

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief プレイヤーのカードUI描画
	/// @param  
	void Draw(void)override;


#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG


private:



	//楕円の半径
	static constexpr float RADIUS_X = 186.0f;	//横半径
	//static constexpr float RADIUS_X = 170.0f;	//横半径
	static constexpr float RADIUS_Y = 214.0f;	//横半径
	//楕円中心
	static constexpr float CENTER_X = 80.0f;
	static constexpr float CENTER_Y = 640.0f;
	///*static constexpr float CENTER_X = 0.0f;*/
	//static constexpr float CENTER_X = 200.0f;
	//static constexpr float CENTER_Y = 440.0f;
	//見せるカード枚数
	static constexpr int VISIBLE_CARD_MAX = 6;
	//カード角度間隔
	static constexpr float VISIBLE_ANGLE_OFFSET = 22.6f;
	//カードセレクト時間
	static constexpr float SELECT_MOVE_CARD_TIME = 0.1f;
	//カード決定UI時間
	static constexpr float DISITION_MOVE_CARD_TIME = SELECT_MOVE_CARD_TIME;

	//先頭に追加するときの戻る枚数
	static constexpr int PREV_CARD_COUNT = 2;
	//始点角度
	static constexpr float START_ANGLE = 0.0f;
	//終点角度
	static constexpr float END_ANGLE = 135.0f;
	//カード角度関連
	static constexpr int ARROUND_NUM = 16;			//一周当たりの枚数		
	static constexpr int ARROUND_NUM_PER_QUAD = ARROUND_NUM / 4;//90度当たりの枚数
	static constexpr float ARROUND_PER_DEG = 360.0f / ARROUND_NUM;	//１枚当たりの角度
	static constexpr float ARROUND_PER_RAD = ARROUND_PER_DEG * DX_PI_F / 180.0f;//ラジアン変換
	static constexpr float ARROUND_PER_QUAD_DEG = ARROUND_PER_DEG * ARROUND_NUM_PER_QUAD;//90度当たりの枚数
	static constexpr float ARROUND_PER_QUAD_RAD = ARROUND_PER_QUAD_DEG * DX_PI_F / 180.0f;//90度当たりの枚数

	//手札の選択カードと

	//選択カード番号
	static constexpr int SELECT_CARD_NO = 1;

	////初期カード
	//std::vector<CARD_UI_INFO>uiInfos_;

	Vector2F radius_;

	////カードUI状態
	//std::function<void(void)>cardUpdate_;
	////状態遷移
	//std::map<CARD_SELECT, std::function<void(void)>>changeMoveState_;

	//見せるカード
	std::list<CARD_UI_INFO>visibleCards_;
	//アクション中カード
	std::list<CARD_UI_INFO>actions_;
	//円形UIの中心座標
	Vector2 centerPos_;
	////カード番号イメージ
	//int* cardNoImgs_;
	////攻撃カード画像
	//int atkCardImg_;
	////リロードカード画像
	//int reloadCardImg_;
	//カードナンバー座標
	Vector2F numPos_;
	//カードセレクトの動き時間
	float cardMoveCnt_;
	//決定後のカウント
	float disitionCnt_;
	////カードのサイズの動き時間
	//double sclSmallCnt_;
	

	//現在選択中のカード
	std::list<CARD_UI_INFO>::iterator visibleCurrent_;

	//カード初期化
	void InitCardUI(void)override;
	//カード状態遷移
	void ChangeNone(void)override;		//通常
	void ChangeLeft(void)override;		//左に移動
	void ChangeRight(void)override;		//右に移動
	void ChangeDisition(void)override;	//決定
	void ChangeReloadWait(void)override;

	void UpdateNone(void)override;
	void UpdateLeft(void)override;
	void UpdateRight(void)override;
	void UpdateDisition(void)override;
	void UpdateReloadWait(void)override;

	
	//すべてのカードの移動
	void MoveCardAll(void);
	//特定のカードのみの移動
	void MoveSpecificCard(CARD_UI_INFO& _card);



	//角度を現在角度に設定
	void CurrentAngle(void);
	//現在選択中のカード更新
	void UpdateVisibleCurrent(void);
	//見せるカードの更新
	void UpdateVisibleCard(void);
	//カード番号座標の追従
	void UpdateCardNumPost(void);
	//手札の消去
	void EraseHandCard(void);
	//カード使用時のカード角度の更新
	void DesideGoalAngle(void);

	////使用済みのカードを消す
	//void UpdateUsedCard(void);

};

