#pragma once

#include "../Base/HpUIBase.h"
class PlayerHpUI:public HpUIBase
{
public:

	//バー座標
	static constexpr Vector2F BAR_POS = { 26.0f, 23.0f };
	static constexpr Vector2F BAR_COVER_POS = { 0.0f, 20.0f };

	PlayerHpUI(void);
	~PlayerHpUI(void)override;
	void Load(void) override;
	void Init(void) override;
	void Update(void)override;
	void Draw(void)override;
private:
	//ゲージサイズ
	static constexpr Vector2F GAUGE_SIZE = { 200.0f,200.0f };

	//円形の部分で示すHp割合
	static constexpr float ARC_HP_PER = 0.7f;
	//線形
	static constexpr float LINE_HP_PER = 0.3f;

	//線形のYサイズ
	static constexpr int LINE_SIZE_Y = 34;


	//バーの色(明るい緑)
	static constexpr FLOAT4 BAR_LIGHT_GREEN = { 0.2f, 0.6f, 1.0f,0.0f };
	//バーの色(青)
	static constexpr FLOAT4 BAR_BLUE = { 0.2f, 1.0f, 0.4f,0.0f };


	//レンダラー
	std::unique_ptr<PixelRenderer>lineRenderer_;
	//マテリアル
	std::unique_ptr<PixelMaterial>lineMaterial_;
	//円形ゲージのマスク
	int radiusHpBarMaskImg_;
	//直線
	int lineHpBarMaskImg_;
	int lineHpFrameImg_;

	//デバッグ用
	float per_;
};

