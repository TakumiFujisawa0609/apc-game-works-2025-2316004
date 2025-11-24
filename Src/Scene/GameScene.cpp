#include "GameScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Character/Enemy/Enemy.h"
#include "../Object/Card/CardSystem.h"	
#include "../Object/Stage.h"	

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
	CharacterManager::GetInstance().Destroy();
	SoundManager::GetInstance().Release();

}

void GameScene::Load(void)
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	//ポーズ画面のリソース
	pauseScene_ = std::make_shared<PauseScene>();
	pauseScene_->Load();

	stage_ = std::make_unique<Stage>();

	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Load();

	CharacterManager::CreateInstance();
	CharacterManager::GetInstance().Load();
}

void GameScene::Init(void)
{
	CollisionManager::CreateInstance();
	CardSystem::CreateInstance();
	CharacterManager::GetInstance().Init();
	//重力管理クラスを生成
	GravityManager::CreateInstance();
	stage_->Init();
	skyDome_->Init();
	//player_->Init();
	//enemy_->Init();
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::GAME_BGM);
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(30.0f, static_cast<int>(SoundManager::TYPE::BGM));


}

void GameScene::NormalUpdate(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		scnMng_.PushScene(pauseScene_);
		return;
	}
	//とりあえず敵が倒れたら
	if (CharacterManager::GetInstance().IsSceneChageClearCondition())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
	}
	else if (CharacterManager::GetInstance().IsSceneChangeGameOverCondition())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_OVER);
	}
	//ステージ
	stage_->Update();

	//キャラクターの更新
	CharacterManager::GetInstance().Update();
	////プレイヤーの更新
	//player_->Update();
	////敵の更新
	//enemy_->Update();
	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();
	//更新はアクション中のみ
	CollisionManager::GetInstance().Update();

#ifdef _DEBUG
	//デバッグ処理
	DebagUpdate();
#endif // _DEBUG


}

void GameScene::NormalDraw(void)
{
	//デバッグ処理
	DebagDraw();

	//プレイヤーの描画
	//PlayerManager::GetInstance().DrawPlayerUI();
	skyDome_->Draw();
	stage_->Draw();
	CharacterManager::GetInstance().Draw();

#ifdef _DEBUG
	//CardSystem::GetInstance().DrawDebug();
#endif // _DEBUG

	//player_->DrawPlayerUI();
	//enemy_->DrawPlayerUI();
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
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
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
