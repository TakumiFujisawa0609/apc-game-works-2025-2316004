#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&GameClearScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameClearScene::LoadingDraw, this);
}

GameClearScene::~GameClearScene(void)
{
}

void GameClearScene::Load(void)
{
}

void GameClearScene::Init(void)
{
}

void GameClearScene::NormalUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	InputManagerS& insS = InputManagerS::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	else if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameClearScene::NormalDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0xff0000,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		L"GameClearScene"
	);
}

void GameClearScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&GameClearScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameClearScene::NormalDraw, this);
}
