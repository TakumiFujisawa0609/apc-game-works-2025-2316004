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
class PixelRenderer;
class PixelMaterial;

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


	//バーの色(明るい緑)
	static constexpr FLOAT4 BAR_LIGHT_GREEN = { 0.2f, 0.6f, 1.0f,0.0f };
	//バーの色(青)
	static constexpr FLOAT4 BAR_BLUE = { 0.2f, 1.0f, 0.4f,0.0f };
	//ゲージ座標
	static constexpr Vector2F BAR_POS = { 50.0f,Application::SCREEN_SIZE_Y - 70 };
	//static constexpr Vector2F BAR_POS0 = { 0,0 };
	// 
	//ゲージの大きさ
	static constexpr float BAR_SCALE = 0.1f;
	//マスクサイズ
	static constexpr Vector2F BAR_SIZE = { 1095,618 };

	//弾かれる前のゴール座標
	static constexpr Vector2F REACT_GOAL_CARD_POS = {-200.0f, Application::SCREEN_HALF_Y + 500 };
	//カード残り枚数ゲージシェーダ定数バッファサイズ
	static constexpr int CARD_NUM_GAUGE_CONST_BUF_SIZE = 3;
	//カード残り枚数ゲージシェーダ定数バッファインデックス
	static constexpr int CARD_NUM_GAUGE_CONST_BUF_IDX = 2;
	//選択カード番号
	static constexpr int SELECT_CARD_NO = 1;
	//フォントサイズ
	static constexpr int FONT_SIZE = 32;
	//フォントの輪郭幅
	static constexpr int FONT_EDGE_SIZE = 2;
	static constexpr Vector2 FONT_POS = { 50,550 };
	////初期カード
	//std::vector<CARD_UI_INFO>uiInfos_;

	Vector2F radius_;

	std::list<std::shared_ptr<CardUIController>>visibleCards_;				//見せるカード
	std::list<std::shared_ptr<CardUIController>>::iterator reloadAnimCurr_;	//リロード用の現在のカードイテレータ
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
	//リロード終了
	bool isReloadEnd_;

	//残りカード枚数ゲージ
	float cardNumPer_;
	//残りカード枚数ゲージ
	int cardNumFrameImg_;
	//残りカード枚数マスク画像
	int cardNumMaskImg_;
	//カード残り枚数ゲージの大きさ
	Vector2F barSize_;
	//フォント
	int fontHandle_;

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
	void SetBasePosVisibleCards(void);

	//カードゲージ(残り枚数描画)
	void DrawCardNum(void);
	
	//見せるカードの更新
	void UpdateVisibleCard(void);
	//手札の消去
	void EraseHandCard(void);
	//カード使用時のカード角度の更新
	void DesideGoalAngle(void);
	//リロードアニメーション
	void ReloadAnimation(void);
	////使用済みのカードを消す
	void ReloadCardArray(void);

	//上下に見せカードを動かす
	void MoveUpDownVisibleCards(void);

	//現在選択中のカードの配列を取得
	std::list<std::shared_ptr<CardUIController>>::iterator GetVisibleCurrentIt(void);
	std::list<std::shared_ptr<CardUIController>>::iterator GetSearchHandIt(std::shared_ptr<CardUIController> target);

	//カード残り枚数のゲージ
	std::unique_ptr<PixelMaterial> cardGaugePSMaterial_;
	std::unique_ptr<PixelRenderer> cardGaugePSRenderer_;

};

