#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void):
	soundMng_(SoundManager::GetInstance())
{
	//更新関数のセット
	updateFunc_ = std::bind(&GameOverScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameOverScene::LoadingDraw, this);
}

GameOverScene::~GameOverScene(void)
{
	soundMng_.Stop(SoundManager::SRC::GAME_OVER);
}

void GameOverScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	imgGameOver_ = resMng_.GetInstance().Load(ResourceManager::SRC::GAME_OVER_IMG).handleId_;

	//BGMロード
	soundMng_.GetInstance().LoadResource(SoundManager::SRC::GAME_OVER);

}

void GameOverScene::Init(void)
{
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FIXED_POINT);

	//BGM再生
	soundMng_.GetInstance().Play(SoundManager::SRC::GAME_OVER, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(BGM_VOL, static_cast<int>(SoundManager::TYPE::BGM));

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
		buttonFontHandle_,
		L"'Bボタンまたはスペースキー'でタイトルに戻る"
	);
}

void GameOverScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = std::bind(&GameOverScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameOverScene::NormalDraw, this);
}
