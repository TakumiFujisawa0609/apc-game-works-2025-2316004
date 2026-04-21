#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Manager/Generic/MenuController.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/Easing.h"
#include "TitleScene.h"

TitleScene::TitleScene(void):
	soundMng_(SoundManager::GetInstance())
{
	//更新関数のセット
	updateFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);

	menuController_ = std::make_unique<MenuController>();
}

TitleScene::~TitleScene(void)
{
	soundMng_.Stop(SoundManager::SRC::TITLE_BGM);
}

void TitleScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	//タイトル画像の読み込み
	imgTitleBack = resMng_.Load(ResourceManager::SRC::TITLE_BACK_IMG).handleId_;

	//タイトルロゴの読み込み
	imgTitleLogo = resMng_.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	menuController_->LoadFont(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE);

	resMng_.Load(ResourceManager::SRC::TITLE_BGM);
	resMng_.Load(ResourceManager::SRC::MOVE_BTN_SE);
	resMng_.Load(ResourceManager::SRC::DESIDE_BTN_SE);
	resMng_.Load(ResourceManager::SRC::GAME_START_SE);

	ButtonUIManager::GetInstance().Load();


	yesNoState_ = YES_NO::NO;
}

void TitleScene::Init(void)
{
	titleSelectFuncTable_ = {
		{TITLE_STATE::EASE_MENU,[this]() {UpdateEase();}},
		{TITLE_STATE::MENU,[this]() {UpdateMenu();}},
		{TITLE_STATE::START_GAME,[this]() {UpdateSelectGame();}},
		{TITLE_STATE::EXIT_MENU,[this]() {UpdateExitMenu();}},
		{TITLE_STATE::SCREEN,[this]() {UpdateScreen();}},
		{TITLE_STATE::EXIT,[this](){ Application::GetInstance().IsGameEnd(); }}
	};

	buttonStrTable_ = {
		{TITLE_BTN::START_GAME,L"START　GAME"},
		{TITLE_BTN::SCREEN,L"SCREEN"},																							
		{TITLE_BTN::EXIT,L"EXIT"}
	};


	yesNoStrTable_ = {
		{YES_NO::YES,L"はい"},
		{YES_NO::NO,L"いいえ"}
	};
	for (int i = 0; i < static_cast<int>(TITLE_BTN::MAX); i++)
	{
		BTN btn = {};
		Vector2F pos = { Application::SCREEN_SIZE_X,BUTTON_START_POS_Y + BUTTON_DISTANCE * i };
		btn.btnStr = buttonStrTable_[static_cast<TITLE_BTN>(i)];
		btn.btnType = static_cast<TITLE_BTN>(i);
		btn.startPos = pos;
		btn.curPos = btn.startPos;
		btn.easeCnt = BUTTON_EASING_TIME;
		buttons_.emplace_back(btn);
	}
	easing_ = std::make_unique<Easing>();
	selectState_ = TITLE_STATE::MENU;
	ChangeState(TITLE_STATE::EASE_MENU);
	selectNum_ = 0;
	easeDistanceCnt_ = 0.0f;
	logoPos_ = { -LOGO_SIZE_X,-LOGO_SIZE_Y };
	logoEaseCnt_ = BUTTON_EASING_TIME;

	int i = 0;
	for (auto& button : buttonStrTable_)
	{
		//イージング演出をするために初期位置は画面外にする
		Vector2 pos = { Application::SCREEN_SIZE_X,static_cast<int>(BUTTON_START_POS_Y+ BUTTON_DISTANCE*i) };
		menuController_->AddMenu(static_cast<int>(button.first), button.second, pos);
		i++;
	}

	SoundManager::GetInstance().Play(ResourceManager::SRC::TITLE_BGM,SoundManager::PLAYTYPE::LOOP);
}

void TitleScene::ChangeState(const TITLE_STATE& _state)
{
	if (selectState_ == _state)return;
	selectState_ = _state;
	menuController_->SetYesNoUpdate(false);
}

void TitleScene::NormalUpdate(void)
{	
	titleSelectFuncTable_[selectState_]();
}

void TitleScene::NormalDraw(void)
{
	DrawExtendGraph(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgTitleBack,
		true
	);
	
	//タイトルロゴ
	DrawExtendGraphF(logoPos_.x, logoPos_.y, logoPos_.x + LOGO_SIZE_X, logoPos_.y + LOGO_SIZE_Y, imgTitleLogo, true);
	menuController_->Draw();

	if (selectState_ == TITLE_STATE::EXIT_MENU|| selectState_ == TITLE_STATE::SCREEN)
	{
		std::wstring str = L"";
		if (selectState_ == TITLE_STATE::EXIT_MENU)
		{
			str = EXIT_MESSAGE;
		}
		else if (selectState_ == TITLE_STATE::SCREEN)
		{
			bool isFull = DataBank::GetInstance().GetIsFullScreen();
			if (isFull)
			{
				str = CHANGE_NORMAL_SCREEN_MESSAGE;
			}
			else
			{
				str = CHANGE_FULL_SCREEN_MESSAGE;
			}
		}
		menuController_->YesNoDraw(str);
	}


	//決定ボタン
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::B_BUTTON_COL_PUSH, DICITION_BTN_POS, DICITION_BTN_SIZE);
	Vector2F strPos = { DICITION_BTN_POS.x + DICITION_BTN_SIZE / 2.0f,DICITION_BTN_POS.y - BTN_STR_OFFSET_X };
	DrawStringToHandle(
		static_cast<int>(strPos.x),
		static_cast<int>(strPos.y),
		DECIDE_STR.c_str(),
		UtilityCommon::WHITE, buttonFontHandle_);

	//戻るボタン
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::A_BUTTON_COL_PUSH, BACK_BTN_POS, DICITION_BTN_SIZE);
	strPos = { BACK_BTN_POS.x + DICITION_BTN_SIZE / 2.0f,BACK_BTN_POS.y - BTN_STR_OFFSET_X };
	DrawStringToHandle(
		static_cast<int>(strPos.x), 
		static_cast<int>(strPos.y), 
		BACK_STR.c_str(),
		UtilityCommon::WHITE, buttonFontHandle_);

}

void TitleScene::OnSceneEnter(void)
{
	//処理変更
	updateFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

void TitleScene::UpdateEase(void)
{
	logoEaseCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	logoPos_ = easing_->EaseFunc(START_POS, GOAL_POS, (LOGO_EASING_TIME - logoEaseCnt_) / LOGO_EASING_TIME, Easing::EASING_TYPE::ELASTIC_OUT);

	menuController_->UpdateDirection(EASING_DIS_TIME, BUTTON_EASING_TIME, static_cast<int>(BUTTON_START_POS_X));
	if (menuController_->IsAllDirectEaseEnd())
	{
		ChangeState(TITLE_STATE::MENU);
	}
}

void TitleScene::UpdateMenu(void)
{
	//選択中のボタンをイージングで動かす
	menuController_->NormalUpdate(SELECT_EASE_DISTANCE, SELECT_EASE_TIME,Easing::EASING_TYPE::COS_BACK);

	// シーン遷移
	if (inputMngS_.IsTrgDown(INPUT_EVENT::UP)||inputMng_.IsTrgDown(KEY_INPUT_W))
	{
		soundMng_.Play(ResourceManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->SubSelectMenuNum();
	}
	else if (inputMngS_.IsTrgDown(INPUT_EVENT::DOWN) || inputMng_.IsTrgDown(KEY_INPUT_S))
	{
		soundMng_.Play(ResourceManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->AddSelectMenuNum();
	}
	selectNum_ = menuController_->GetSelectMenuNum();

	//OKボタンが押されたら
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		ResourceManager::SRC se;
		//ゲームスタート以外のボタンなら決定音、ゲームスタートならゲームスタート音を鳴らす
		selectNum_ != static_cast<int>(TITLE_BTN::START_GAME) ? se = ResourceManager::SRC::DESIDE_BTN_SE : se = ResourceManager::SRC::GAME_START_SE;
		soundMng_.Play(se, SoundManager::PLAYTYPE::BACK);
		ChangeState(static_cast<TITLE_STATE>(selectNum_));
	}
}

void TitleScene::UpdateScreen(void)
{
	UpdateYesNo();
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		if (menuController_->GetIsYes())
		{
			ChangeScreenSize();
			ChangeState(static_cast<TITLE_STATE>(TITLE_STATE::MENU));
		}
		else { ChangeState(TITLE_STATE::MENU); }
	}
}

void TitleScene::UpdateTutorial(void)
{
	ChangeState(TITLE_STATE::MENU);
}

void TitleScene::UpdateSelectGame(void)
{
	if (!soundMng_.IsPlay(SoundManager::SRC::GAME_START_SE))
	{
		soundMng_.Play(SoundManager::SRC::GAME_START_SE, SoundManager::PLAYTYPE::BACK);
	}

	//ゲームシーンに遷移
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
}

void TitleScene::UpdateExitMenu(void)
{
	UpdateYesNo();
	if (inputMngS_.IsTrgDown(INPUT_EVENT::OK))
	{
		if (menuController_->GetIsYes())
		{
			Application::GetInstance().IsGameEnd();
		}
		else { ChangeState(TITLE_STATE::MENU); }
	}
}
void TitleScene::UpdateYesNo(void)
{
	if (inputMngS_.IsTrgDown(INPUT_EVENT::LEFT) || inputMng_.IsTrgDown(KEY_INPUT_A))
	{
		soundMng_.Play(ResourceManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->SetYesNoUpdate(true);
	}
	else if (inputMngS_.IsTrgDown(INPUT_EVENT::RIGHT) || inputMng_.IsTrgDown(KEY_INPUT_D))
	{
		soundMng_.Play(ResourceManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->SetYesNoUpdate(false);
	}
}

void TitleScene::ChangeScreenSize(void)
{
	bool isFull = DataBank::GetInstance().GetIsFullScreen();
	if (isFull)
	{
		DataBank::GetInstance().SetIsFullScreen(false);
	}
	else
	{
		DataBank::GetInstance().SetIsFullScreen(true);
	}
}

