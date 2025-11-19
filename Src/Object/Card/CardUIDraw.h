#pragma once
#include"./CardBase.h"

class CardUIController;

class CardUIDraw :public ObjectBase
{

public:

	CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl);
	~CardUIDraw(void)override;
	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:

	int& typeImg_;		//カードの種類画像

	Vector2F size_;		//カードの大きさ
	Vector2F& centerPos_;	//中心座標
	float& scl_;			//サイズ


};

