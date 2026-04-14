#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/UtilityDraw.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Common/Easing.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene(void):
	soundMng_(SoundManager::GetInstance()),
	easeCnt_(0.0f),
	strYPos_(SceneBase::BACK_TITLE_STRING_POS_Y)
{
	//更新関数のセット
	updateFunc_ = std::bind(&GameClearScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameClearScene::LoadingDraw, this);

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FIXED_POINT);
}

GameClearScene::~GameClearScene(void)
{
	soundMng_.Stop(SoundManager::SRC::GAME_CLEAR);
}

void GameClearScene::Load(void)
{
	imgGameClear_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_CLEAR_IMG).handleId_;
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	//BGMロード
	soundMng_.GetInstance().LoadResource(SoundManager::SRC::GAME_CLEAR);
	SoundManager::GetInstance().SetSystemVolume(BGM_VOL, static_cast<int>(SoundManager::TYPE::BGM));
}

void GameClearScene::Init(void)
{

	//BGM再生
	soundMng_.GetInstance().Play(SoundManager::SRC::GAME_CLEAR, SoundManager::PLAYTYPE::LOOP);

	easing_ = std::make_unique<Easing>();

}

void GameClearScene::NormalUpdate(void)
{
	// シーン遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_SPACE)|| inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	const float LIMIT = SceneBase::BACK_TITLE_STRING_POS_Y - BACK_TITLE_STRING_POS_EASE_LIMIT;
	strYPos_ = easing_->EaseFunc(SceneBase::BACK_TITLE_STRING_POS_Y, LIMIT, easeCnt_ / EASING_TIME, Easing::EASING_TYPE::QUAD_BACK);
	easeCnt_ += scnMng_.GetDeltaTime();
	if (easeCnt_ > EASING_TIME)
	{
		easeCnt_ = 0.0f;
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
		0xffffff,
		L"GameClearScene"
	);

	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, imgGameClear_, true);

	UtilityDraw::DrawStringCenter(
		Application::SCREEN_HALF_X,
		static_cast<int>(strYPos_),
		L"'B'ボタンまたは'スペースキー'でタイトルに戻る",
		UtilityCommon::WHITE,
		buttonFontHandle_
	);

}

void GameClearScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = std::bind(&GameClearScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameClearScene::NormalDraw, this);
}
