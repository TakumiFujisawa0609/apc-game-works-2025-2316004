#pragma once
#include "../Common/Vector2F.h"
#include "../Base/HpUIBase.h"


class EnemyHpUI:public HpUIBase
{
public:
	EnemyHpUI(void);
	~EnemyHpUI(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	//バー固定座標
	static constexpr Vector2F BAR_POS = { 550.0f,23.0f };
	static constexpr Vector2F BAR_COVER_POS = { 524.0f, 20.0f };
	//Hpバーサイズ
	static constexpr Vector2F BAR_COVER_SIZE = { 500.0f ,20.0f };
	static constexpr Vector2F BAR_SIZE = { 470.0f ,16.0f };

	//バーの色(明るい青)
	static constexpr FLOAT4 BAR_LIGHT_BLUE = { 0.2f, 0.8f, 1.0f,0.0f };
	//バーの色(紫)
	static constexpr FLOAT4 BAR_PURPLE = { 0.6f, 0.2f, 0.8f,0.0f };

	//減る前のHP保存用定数バッファの番号
	static constexpr int PRE_HP_CONST_BUF = 2;
};

