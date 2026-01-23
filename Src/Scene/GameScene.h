#pragma once
#include <memory>
#include "SceneBase.h"

class PauseScene;
class PixelMaterial;
class PixelRenderer;
class CharacterBase;
class SkyDome;
class Player;
class Enemy;
class Stage;
class CardSystem;

class DrawUI3D;

class GameScene : public SceneBase
{

public:
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

private:

	//BGM
	static constexpr int BGM_GAME_VOL = 50;

	//集中線シェーダー関連
	//定数バッファの個数
	static constexpr int INTENSIVE_CBUFFER_NUM = 1;

	//集中線アニメーション速度
	static constexpr int INTENSIVE_LINE_ANIM_SPEED = 5;

	//ポストエフェクト用スクリーン
	int postEffectScreen_;

	int frame_;
	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//ステージ
	std::unique_ptr<Stage>stage_;

	//ポーズ画面
	std::shared_ptr<PauseScene> pauseScene_;

	//プレイヤー
	std::unique_ptr<Player>player_;

	//敵
	std::unique_ptr<Enemy>enemy_;

	//シェーダクラス
	std::unique_ptr<PixelMaterial> intensiveMaterial_;
	std::unique_ptr<PixelRenderer> intensiveRenderer_;

	//集中線
	int intensiveLineImg_1;
	int intensiveLineImg_2;
	//集中線アニメーション
	int intensiveLineAnimImg_;

	//集中線アニメーションフレーム
	int intensiveLineAnimFrame_;

	//集中線アニメーション更新
	void UpdateIntensiveLineAnim(void);

	//更新関数
	void NormalUpdate(void) override;
	//演出時の更新
	void DirectionUpdate(void);
	//描画関数
	void NormalDraw(void) override;
	//演出時の描画
	void DirectionDraw(void);

	//処理の変更
	void ChangeNormal(void) override;
	
#ifdef _DEBUG
	//デバッグ処理
	void DebagUpdate(void);
	void DebagDraw(void);
#endif // _DEBUG



};