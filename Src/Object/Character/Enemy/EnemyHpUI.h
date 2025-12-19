#pragma once
#include "../Common/Vector2F.h"

class ResourceManager;
class PixelRenderer;
class PixelMaterial;

class EnemyHpUI
{
public:
	EnemyHpUI(float& _hpPer);
	~EnemyHpUI(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	//バー固定座標
	static constexpr Vector2F BAR_POS = { 600.0f,50.0f };

	//Hpバーサイズ
	static constexpr Vector2F BAR_COVER_SIZE = { 500.0f ,20.0f };
	static constexpr Vector2F BAR_SIZE = { 470.0f ,16.0f };

	//Hpバーカバーハンドル
	int barCoverHandle_;
	//HPバーフレーム
	int barFrame_;
	//Hpマスクハンドル
	int hpMask_;

	// プレイヤーの体力の割合
	float& enemyHpPer_;

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


	
};

