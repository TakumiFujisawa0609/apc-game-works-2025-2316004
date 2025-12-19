#pragma once
#include "../Common/Vector2F.h"

class Easing;
class ResourceManager;
class PixelRenderer;
class PixelMaterial;

class EnemyHpUI
{
public:
	EnemyHpUI(float& _hpPer,float& _preHp);
	~EnemyHpUI(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);

	void Shake(void);
private:
	//バー固定座標
	static constexpr Vector2F BAR_POS = { 550.0f,23.0f };
	static constexpr Vector2F BAR_COVER_POS = { static_cast<float>(Application::SCREEN_SIZE_X - 500.0f), 20.0f };
	//Hpバーサイズ
	static constexpr Vector2F BAR_COVER_SIZE = { 500.0f ,20.0f };
	static constexpr Vector2F BAR_SIZE = { 470.0f ,16.0f };

	//補間時間
	static constexpr float LERP_TIME = 0.3f;
	static constexpr float WAIT_TIME = 0.5f;

	//シェイク
	static constexpr float SHAKE_CNT = 0.3f;
	static constexpr float COVER_SHAKE_GOAL_Y= BAR_COVER_POS.y+30.0f;
	static constexpr float BAR_SHAKE_GOAL_Y= BAR_POS.y+30.0f;
	//Hpバーカバーハンドル
	int barCoverHandle_;
	//HPバーフレーム
	int barFrame_;
	//Hpマスクハンドル
	int hpMask_;

	// プレイヤーの体力の割合
	float& enemyHpPer_;

	//線形補間用Hp差分
	float hpLerpPer_;

	float& preHp_;

	//バーカバー座標
	Vector2F barCoverPos_;
	//バー座標
	Vector2F barPos_;
	//リソースマネージャー
	ResourceManager& resMng_;

	//レンダラー
	std::unique_ptr<PixelRenderer>renderer_;
	//マテリアル
	std::unique_ptr<PixelMaterial>material_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//補間カウント
	float lerpCnt_;

	//補間待機時間
	float waitCnt_;

	//シェイクカウント
	float shakeCnt_;


	
};

