#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&GameOverScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameOverScene::LoadingDraw, this);
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Load(void)
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	imgGameOver_ = resMng_.GetInstance().Load(ResourceManager::SRC::GAME_OVER_IMG).handleId_;
}

void GameOverScene::Init(void)
{
}

void GameOverScene::NormalUpdate(void)
{
	if (InputManagerS::GetInstance().IsTrgDown(INPUT_EVENT::OK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::NormalDraw(void)
{
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameOver_, true);

	DrawFormatStringToHandle(
		BACK_TITLE_STRING_POS.x,
		BACK_TITLE_STRING_POS.y,
		UtilityCommon::WHITE,
		buttnFontHandle_,
		L"'Bボタンまたはスペースキー'でタイトルに戻る"
	);
}

void GameOverScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&GameOverScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameOverScene::NormalDraw, this);
}
