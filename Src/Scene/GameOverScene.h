#pragma once
#include "../Common/Vector2.h"
#include "SceneBase.h"
class GameOverScene :
    public SceneBase
{
public:

	/// @brief コンストラクタ
	/// @param  
	GameOverScene(void);

	/// @brief デストラクタ
	/// @param  
	~GameOverScene(void)override;

	/// @brief 読み込み
	/// @param  
	void Load(void) override;
	
	/// @brief 初期化処理
	/// @param  
	void Init(void) override;

private:

	//BGMの大きさ
	static constexpr int BGM_VOL = 80;

	//ゲームオーバー画面
	int imgGameOver_;

	//サウンド
	SoundManager& soundMng_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void OnSceneEnter(void) override;
};

