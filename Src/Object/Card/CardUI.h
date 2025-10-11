#pragma once
#include<functional>
#include<map>
#include<memory>
#include<vector>
#include<list>
#include"./CardBase.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector2F.h"
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
	};

	struct CARD_UI_INFO
	{
		Vector2F cardPos={};
		Vector2F numPos = {};
		int typeImg = -1;
		float currentAngle = 0.0f;
		float goalAngle = currentAngle;
		CardBase::CARD_STATUS status;
		CARD_STATE state=CARD_STATE::DRAW_PILE;
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
	void CardMoveSelect(const CARD_SELECT _select);

private:

	//倍率1の時のカードの強さの描画座標
	static constexpr Vector2F NUM_LOCAL_POS = { 35.0f,53.0f };
	//楕円の半径
	static constexpr float RADIUS_X = 186.0f;	//横半径
	static constexpr float RADIUS_Y = 214.0f;	//横半径
	//楕円中心
	static constexpr float CENTER_X = 50.0f;
	static constexpr float CENTER_Y = 640.0f;
	//static constexpr float CENTER_X = 200.0f;
	//static constexpr float CENTER_Y = 440.0f;
	//見せるカード枚数
	static constexpr int VISIBLE_CARD_MAX = 6;
	//カード角度間隔
	static constexpr float VISIBLE_ANGLE_OFFSET = 22.6f;
	//カードセレクト時間
	static constexpr float SELECT_MOVE_CARD_TIME = 0.1f;
	//始点角度
	static constexpr float START_ANGLE = 0.0f;
	//終点角度
	static constexpr float END_ANGLE = 135.0f;

	//カードのステータス
	std::vector<CARD_UI_INFO>uiInfos_;

	Vector2F radius_;

	//カードUI状態
	std::function<void(void)>cardUpdate_;
	//状態遷移
	std::map<CARD_SELECT, std::function<void(void)>>changeMoveState_;

	//見せるカード
	std::list<CARD_UI_INFO>visibleCards_;
	//円形UIの中心座標
	Vector2 centerPos_;
	//カード番号イメージ
	int* cardNoImgs_;
	//攻撃カード画像
	int atkCardImg_;
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
	//末尾のカードインデックス
	int visibleEndCardIndex_;
	//先頭のカードインデックス
	int visibleStartCardIndex_;

	//カード状態遷移
	void ChangeNone(void);
	void ChangeLeft(void);
	void ChangeRight(void);

	void UpdateNone(void);
	void UpdateLeft(void);
	void UpdateRight(void);


};

