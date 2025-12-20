#pragma once

#include "../Base/HpUIBase.h"
class PlayerHpUI:public HpUIBase
{
public:
	PlayerHpUI(float& _hpPer, float& _preHp);
	~PlayerHpUI(void)override;
	void Load(void) override;
	void Init(void) override;
	void Update(void)override;
	void Draw(void)override;
private:
	//ゲージサイズ
	static constexpr Vector2F GAUGE_SIZE = { 200.0f,200.0f };


	//レンダラー
	std::unique_ptr<PixelRenderer>lineRenderer_;
	//マテリアル
	std::unique_ptr<PixelMaterial>lineMaterial_;
	//円形ゲージのマスク
	int radiusHpBarMaskImg_;
	//直線
	int lineHpBarMaskImg_;

	//デバッグ用
	float per_;
};

