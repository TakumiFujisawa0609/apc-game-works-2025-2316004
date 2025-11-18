#pragma once
#include"./CardBase.h"

class CardUIController;

class CardUI
{

public:

	CardUI(Vector2F& _centerPos, float& _scl);
	~CardUI(void);

	void Draw(void);
private:

	int typeImg_;		//カードの種類画像
	CardBase::CARD_STATUS status_;	//カードのステータス

	Vector2F size_;		//カードの大きさ
	Vector2F& centerPos_;	//中心座標
	float& scl_;			//サイズ

};

