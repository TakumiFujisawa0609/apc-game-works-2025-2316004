#pragma once
#include<memory>
#include<vector>
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

	struct CARD_UI_INFO
	{
		Vector2F cardPos={};
		Vector2F numPos = {};
		int typeImg = -1;
		float angle = 0.0f;
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

private:

	//倍率1の時のカードの強さの描画座標
	static constexpr Vector2F NUM_LOCAL_POS = { 35.0f,53.0f };
	//半径
	static constexpr float RADIUS = 200.0f;
	//カードのステータス
	std::vector<CARD_UI_INFO>uiInfos_;
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
};

