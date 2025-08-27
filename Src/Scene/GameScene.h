#pragma once
#include <memory>
#include "SceneBase.h"

class PauseScene;
class Enemy;
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

	int frame_;

	//ポーズ画面
	std::shared_ptr<PauseScene> pauseScene_;

	//敵
	std::unique_ptr<Enemy>enemy_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;
	
	//デバッグ処理
	void DebagUpdate(void);
	void DebagDraw(void);

};