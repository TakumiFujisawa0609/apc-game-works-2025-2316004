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

class CardUI;
class CardUIController;

class PlayerCardUI
	:public CardUIBase
{

public:

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

	//カード初期化
	void InitCardUI(void)override;

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG


private:



	//楕円の半径
	static constexpr float RADIUS_X = 186.0f;	//横半径
	//static constexpr float RADIUS_X = 170.0f;	//横半径
	static constexpr float RADIUS_Y = 214.0f;	//横半径




	//楕円中心
	//static constexpr float CENTER_X = 80.0f;
	//static constexpr float CENTER_Y = 640.0f;
	///*static constexpr float CENTER_X = 0.0f;
	//static constexpr float CENTER_X = 200.0f;
	//static constexpr float CENTER_Y = 440.0f;
	//見せるカード枚数
	static constexpr int VISIBLE_CARD_MAX = 6;
	//カード角度間隔
	static constexpr float VISIBLE_ANGLE_OFFSET = 22.6f;
	//カードセレクト時間
	//static constexpr float SELECT_MOVE_CARD_TIME = 0.5f;
	//カード決定UI時間
	//static constexpr float DISITION_MOVE_CARD_TIME = SELECT_MOVE_CARD_TIME;

	//先頭に追加するときの戻る枚数
	static constexpr int PREV_CARD_COUNT = 2;
	//現在カードの前にある枚数
	static constexpr int CARDS_BEFORE_CURRENT = 1;
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

	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_GOAL_CARD_POS = {  - 200.0f, Application::SCREEN_HALF_Y + 500 };
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
	//std::list<CARD_UI_INFO>visibleCards_;
	std::list<std::shared_ptr<CardUIController>>visibleCards_;
	std::list<std::shared_ptr<CardUI>>visibleDrawCard_;			//実際に描画するカード
	std::list<std::shared_ptr<CardUI>>::iterator visibleDrawIt_;			//実際に描画するカード
	//選択フレーム画像
	int selectFrameImg_;
	//リロードフレーム
	int reloadCardFrameImg_;
	//円形UIの中心座標
	Vector2 centerPos_;

	//カードナンバー座標
	Vector2F numPos_;
	//カードセレクトの動き時間
	float cardMoveCnt_;
	
	//補完角度リスト(左)
	std::list<float>goalLeftRad_;
	//補完角度リスト(右)
	std::list<float>goalRightRad_;
	//リロード終了
	bool isReloadEnd_;



	//std::unique_ptr<CardUI>uiDraw_;
	std::unique_ptr<CardUIController>uiController_;


	//現在選択中のカード
	std::list<std::shared_ptr<CardUIController>>::iterator visibleCurrent_;
	//リロード用の現在のカードイテレータ
	std::list<std::shared_ptr<CardUIController>>::iterator reloadAnimCurr_;


	//カード状態遷移
	void ChangeNone(void)override;		//通常
	void ChangeLeft(void)override;		//左に移動
	void ChangeRight(void)override;		//右に移動
	void ChangeDecision(void)override;	//決定
	void ChangeReloadWait(void)override;
	void ChangeReload(void);

	void UpdateNone(void)override;
	void UpdateLeft(void)override;
	void UpdateRight(void)override;
	void UpdateDecision(void)override;
	void UpdateReloadWait(void)override;
	void UpdateReload(void);

	
	//すべてのカードの移動
	void MoveCardAll(const float& _moveTImeMax);
	//カードUI描画オブジェクト更新
	void UpdateDrawCardUI(void) override;



	//角度を現在角度に設定
	void CurrentAngle(void);
	//現在選択中のカード更新
	//void UpdateVisibleCurrent(void);
	//見せるカードの更新
	void UpdateVisibleCard(void);
	//手札の消去
	void EraseHandCard(void);
	//カード使用時のカード角度の更新
	void DesideGoalAngle(void);
	//リロードアニメーション
	void ReloadAnimation(void);
	////使用済みのカードを消す
	//void UpdateUsedCard(void);


	std::list<std::shared_ptr<CardUIController>>::iterator GetVisibleCurrentIt(void);
	std::list<std::shared_ptr<CardUIController>>::iterator GetSearchHandIt(std::shared_ptr<CardUIController> target);

};

