#pragma once
#include<functional>
#include<map>
#include<memory>
#include<vector>
#include<list>
#include"./CardBase.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
#include"../Application.h"
class CardUI
{

public:
	enum class CARD_STATE
	{
		DRAW_PILE
		, MOVE_DRAW
		, USING
		, REACT
	};

	enum class CARD_SELECT
	{
		NONE
		, LEFT
		, RIGHT
		, DISITION
	};

	struct CARD_UI_INFO
	{
		Vector2F cardPos = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの座標(画面外で初期化)
		Vector2F numPos = { Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 };		//カードの強さ番号座標(画面外で初期化)
		int typeImg = -1;																				//カードの種類画像
		float currentAngle = 0.0f;																		//カードの現在の角度
		float goalAngle = currentAngle;																	//カードの目標の角度
		CardBase::CARD_STATUS status;																	//カードのステータス
		CARD_STATE state = CARD_STATE::DRAW_PILE;														//カードの状態
	};

	/// @brief コンストラクタ
	/// @param  
	CardUI(void);

	//デストラクタ
	~CardUI(void);

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief UI配列に追加
	/// @param _status 
	void AddCardUi(const CardBase::CARD_STATUS _status);

	/// @brief カード状態選択
	/// @param _select 
	void ChangeSelectState(const CARD_SELECT _select);

	/// @brief カードの決定
	/// @param  
	void CardDisition(void);

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG


private:

	//倍率1の時のカードの強さの描画座標
	static constexpr Vector2F NUM_LOCAL_POS = { 35.0f,53.0f };
	//決定したカードの座標
	static constexpr Vector2F DISITON_CARD_POS = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 200 };
	//楕円の半径
	static constexpr float RADIUS_X = 186.0f;	//横半径
	static constexpr float RADIUS_Y = 214.0f;	//横半径
	//楕円中心
	//static constexpr float CENTER_X = 50.0f;
	//static constexpr float CENTER_Y = 640.0f;
	///*static constexpr float CENTER_X = 0.0f;*/
	static constexpr float CENTER_X = 200.0f;
	static constexpr float CENTER_Y = 440.0f;
	//見せるカード枚数
	static constexpr int VISIBLE_CARD_MAX = 6;
	//カード角度間隔
	static constexpr float VISIBLE_ANGLE_OFFSET = 22.6f;
	//カードセレクト時間
	static constexpr float SELECT_MOVE_CARD_TIME = 0.1f;
	//カード決定UI時間
	static constexpr float DISITION_MOVE_CARD_TIME = 0.1f;
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

	//初期カード
	std::vector<CARD_UI_INFO>uiInfos_;

	Vector2F radius_;

	//カードUI状態
	std::function<void(void)>cardUpdate_;
	//状態遷移
	std::map<CARD_SELECT, std::function<void(void)>>changeMoveState_;
	//手札
	std::list<CARD_UI_INFO>handCards_;
	//見せるカード
	std::list<CARD_UI_INFO>visibleCards_;
	//アクション中カード
	std::list<CARD_UI_INFO>actions_;
	//円形UIの中心座標
	Vector2 centerPos_;
	//カード番号イメージ
	int* cardNoImgs_;
	//攻撃カード画像
	int atkCardImg_;
	//リロードカード画像
	int reloadCardImg_;
	//カードの大きさ
	double cardScl_;
	//カードの座標
	Vector2F cardPos_;
	//カードナンバー座標
	Vector2F numPos_;
	//カードセレクトの動き時間
	float cardMoveCnt_;
	//状態
	CARD_SELECT selectState_;
	//セレクト中のカード配列
	int selectCardIndex_;
	//決定後のカウント
	float disitionCnt_;
	//現在選択中のカード
	std::list<CARD_UI_INFO>::iterator current_;
	//手札の現在選択中カード
	std::list<CARD_UI_INFO>::iterator handCurrent_;

	//末尾に入れるカードインデックス
	int visibleEndPushIndex_;
	//先頭のカードインデックス
	int visibleStartCardIndex_;

	//カード状態遷移
	void ChangeNone(void);		//通常
	void ChangeLeft(void);		//左に移動
	void ChangeRight(void);		//右に移動
	void ChangeDisition(void);	//決定

	void UpdateNone(void);
	void UpdateLeft(void);
	void UpdateRight(void);
	void UpdateDisition(void);

	//すべてのカードの移動
	void MoveCardAll(void);
	//特定のカードのみの移動
	void MoveSpecificCard(CARD_UI_INFO& _card);

	//範囲インデックスの計算
	void AddIndex(int& _index);	//足し算
	void SubIndex(int& _index);	//引き算


	//手札選択カードの計算
	void AddHandCurrent(void);	//足し算
	void SubHandCurrent(void);	//引き算

	//使い終わったカードを配列から削除
	void EraseArray(std::list<CARD_UI_INFO>& _list, std::list<CARD_UI_INFO>::iterator _itr);
	
	//カード描画
	void DrawCard(const CARD_UI_INFO _card);
	//角度を現在角度に設定
	void CurrentAngle(void);
};

