#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontManager::FONT_DOT.c_str(), FONT_SIZE, 0);
	//タイトル画像の読み込み
	titleImg_ = resMng_.Load(ResourceManager::SRC::TITLE_IMG).handleId_;

	titleFont_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	yesNoState_ = YES_NO::NO;
}

void TitleScene::Init(void)
{
	titleSelectFuncTable_ = { 
		{TITLE_STATE::MENU,[this]() {
			UpdateMenu();
		}},
		{TITLE_STATE::START_GAME,[this]() {
			UpdateSelectGame();
		}},
		{TITLE_STATE::TUTORIAL,[this]() { /* チュートリアル処理 */ } },
		{TITLE_STATE::EXIT_MENU,[this]() {UpdateExitMenu();}},
		{TITLE_STATE::EXIT,[this](){ Application::GetInstance().IsGameEnd(); }}
	};

	buttonStrTable_ = {
		{TITLE_BTN::START_GAME,L"START　GAME"},
		{TITLE_BTN::TUTORIAL,L"TUTORIAL"},
		{TITLE_BTN::EXIT,L"EXIT"}
	};

	yesNoStrTable_ = {
		{YES_NO::YES,L"はい"},
		{YES_NO::NO,L"いいえ"}
	};

	selectState_ = TITLE_STATE::MENU;
	selectNum_ = 0;
}

void TitleScene::ChangeState(const TITLE_STATE& _state)
{
	if (selectState_ == _state)return;
	selectState_ = _state;
	yesNoState_ = YES_NO::NO;
}

void TitleScene::NormalUpdate(void)
{	
	titleSelectFuncTable_[selectState_]();
}

void TitleScene::NormalDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x0000ff,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		L"TitleScene"
	);


	DrawExtendGraph(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		titleImg_,
		true
	);

	int i = 0;
	for (auto& string : buttonStrTable_)
	{
		unsigned int btnCol = UtilityCommon::WHITE;
		if (selectNum_ == static_cast<int>(string.first))
		{
			btnCol = UtilityCommon::RED;
		}
		DrawFormatStringToHandle(
			BUTTON_START_POS_X,
			BUTTON_START_POS_Y + BUTTON_DISTANCE * i,
			btnCol,
			titleFont_,
			string.second.c_str()
		);
		i++;
	}

	if (selectState_ == TITLE_STATE::EXIT_MENU)
	{
		const Vector2 startPos = { Application::SCREEN_HALF_X - (CHECK_EXIT_MENU_SIZE_X / 2)
									,Application::SCREEN_HALF_Y - (CHECK_EXIT_MENU_SIZE_Y / 2) };
		const Vector2 endPos = { Application::SCREEN_HALF_X + (CHECK_EXIT_MENU_SIZE_X / 2),
								Application::SCREEN_HALF_Y + (CHECK_EXIT_MENU_SIZE_Y / 2) };

		//メニュ背景
		DrawBox(startPos.x,
			startPos.y,
			endPos.x,
			endPos.y,
			0x00ff00,true
		);

		DrawFormatStringToHandle(
			startPos.x + QUESTION_OFFSET,
			startPos.y + QUESTION_OFFSET,
			UtilityCommon::WHITE,
			titleFont_,
			L"本当にゲームを終了しますか？"
		);

		int i = 0;
		for (auto& str:yesNoStrTable_)
		{
			unsigned int btnCol = UtilityCommon::WHITE;
			if(yesNoState_==str.first){ btnCol = UtilityCommon::RED; }
			DrawFormatStringToHandle(
				startPos.x + 70+i* YES_NO_DISTANCE,
				startPos.y + 100,
				btnCol,
				titleFont_,
				str.second.c_str()
			);
			i++;
		}
	}

}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

void TitleScene::UpdateMenu(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	InputManagerS& insS = InputManagerS::GetInstance();

	//selectNum_ = static_cast<int>(selectState_);

	if (insS.IsTrgDown(INPUT_EVENT::UP))
	{
		selectNum_--;
	}
	else if (insS.IsTrgDown(INPUT_EVENT::DOWN))
	{
		selectNum_++;
	}
	//０以下にならないように
	if (selectNum_ < 0) { selectNum_ = static_cast<int>(TITLE_BTN::EXIT); }

	selectNum_ %= static_cast<int>(TITLE_BTN::MAX);
	//selectState_ = static_cast<TITLE_STATE>(selectNum_);
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		ChangeState(static_cast<TITLE_STATE>(selectNum_));
	}
}

void TitleScene::UpdateSelectGame(void)
{
	//ゲームシーンに遷移
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
}

void TitleScene::UpdateExitMenu(void)
{
	InputManagerS& insS = InputManagerS::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::LEFT))
	{ 
		yesNoState_ = YES_NO::YES; 
	}
	else if(insS.IsTrgDown(INPUT_EVENT::RIGHT))
	{ 
		yesNoState_ = YES_NO::NO; 
	}

	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		if (yesNoState_ == YES_NO::YES) { 
			Application::GetInstance().IsGameEnd(); 
		}
		else { ChangeState(TITLE_STATE::MENU);}
	}
}

