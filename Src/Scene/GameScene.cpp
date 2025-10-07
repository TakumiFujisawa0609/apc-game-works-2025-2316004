#include "GameScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include"../Manager/Game/CollisionManager.h"
#include "../Manager/Resource/FontManager.h"

#include"../Object/Card/CardSystem.h"
#include"../Manager/Generic/Camera.h"
#include"../Object/Character/Enemy/Enemy.h"	
#include"../Object/Stage.h"	

#include "PauseScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	CardSystem::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();

}

void GameScene::Load(void)
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontManager::FONT_DOT.c_str(), FONT_SIZE, 0);

	//ポーズ画面のリソース
	pauseScene_ = std::make_shared<PauseScene>();
	pauseScene_->Load();

	//PlayerManager::CreateInstance();
	//PlayerManager::GetInstance().Load();
	stage_ = std::make_unique<Stage>();
	

	player_ = std::make_unique<Player>();
	player_->Load();

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);
	SceneManager::GetInstance().GetCamera().lock()->SetFollow(&player_->GetTransform());

	enemy_ = std::make_unique<Enemy>(*player_);
	enemy_->Load();
}

void GameScene::Init(void)
{
	CollisionManager::CreateInstance();
	CardSystem::CreateInstance();
	stage_->Init();
	player_->Init();
	enemy_->Init();

}

void GameScene::NormalUpdate(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		scnMng_.PushScene(pauseScene_);
		return;
	}
	
	//プレイヤーの更新
	player_->Update();
	//敵の更新
	enemy_->Update();
	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();
	//更新はアクション中のみ
	CollisionManager::GetInstance().Update();
	//デバッグ処理
	DebagUpdate();
}

void GameScene::NormalDraw(void)
{
	//デバッグ処理
	DebagDraw();

	//プレイヤーの描画
	//PlayerManager::GetInstance().Draw();
	stage_->Draw();
	player_->Draw();
	enemy_->Draw();
}

void GameScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&GameScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameScene::NormalDraw, this);
}

void GameScene::DebagUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	frame_++;
}

void GameScene::DebagDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x00ff00,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		L"GameScene"
	);

	constexpr float r = 40.0f;
	float angle = DX_PI_F * 2.0f * static_cast<float>(frame_ % 360) / 60.0f;

	////円運動を描画
	//DrawCircleAA(
	//	320+cos(angle) * r, 
	//	240+sin(angle) * r,
	//	r, 
	//	32, 
	//	0xff8888, 
	//	true);
}
