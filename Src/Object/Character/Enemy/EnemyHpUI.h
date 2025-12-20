#pragma once
#include "../Common/Vector2F.h"
#include "../Base/HpUIBase.h"


class EnemyHpUI:public HpUIBase
{
public:
	EnemyHpUI(float& _hpPer,float& _preHp);
	~EnemyHpUI(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	//バー固定座標
	static constexpr Vector2F BAR_POS = { 550.0f,23.0f };
	static constexpr Vector2F BAR_COVER_POS = { static_cast<float>(Application::SCREEN_SIZE_X - 500.0f), 20.0f };
	//Hpバーサイズ
	static constexpr Vector2F BAR_COVER_SIZE = { 500.0f ,20.0f };
	static constexpr Vector2F BAR_SIZE = { 470.0f ,16.0f };

};

