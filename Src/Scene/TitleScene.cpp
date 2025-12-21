#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/InputManagerS.h"
#include "../Manager/Generic/DataBank.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Common/Easing.h"
#include "TitleScene.h"

TitleScene::TitleScene(void):
	soundMng_(SoundManager::GetInstance())
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
	buttnFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	//タイトル画像の読み込み
	imgTitleBack = resMng_.Load(ResourceManager::SRC::TITLE_BACK_IMG).handleId_;

	//タイトルロゴの読み込み
	imgTitleLogo = resMng_.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	titleFont_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	soundMng_.LoadResource(SoundManager::SRC::TITLE_BGM);
	soundMng_.LoadResource(SoundManager::SRC::MOVE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::DESIDE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::GAME_START_SE);


	yesNoState_ = YES_NO::NO;
}

void TitleScene::Init(void)
{
	titleSelectFuncTable_ = {
		{TITLE_STATE::EASE_MENU,[this]() {UpdateEase();}},
		{TITLE_STATE::MENU,[this]() {UpdateMenu();}},
		{TITLE_STATE::START_GAME,[this]() {UpdateSelectGame();}},
		{TITLE_STATE::TUTORIAL,[this]() { UpdateTutorial(); }},
		{TITLE_STATE::SCREEN,[this]() { UpdateScreen(); }},
		{TITLE_STATE::EXIT_MENU,[this]() {UpdateExitMenu();}},
		{TITLE_STATE::EXIT,[this](){ Application::GetInstance().IsGameEnd(); }}
	};

	buttonStrTable_ = {
		{TITLE_BTN::START_GAME,L"START　GAME"},
		{TITLE_BTN::TUTORIAL,L"TUTORIAL"},
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

	SoundManager::GetInstance().Play(SoundManager::SRC::TITLE_BGM,SoundManager::PLAYTYPE::LOOP);
}

Easing::EASING_TYPE TitleScene::DecideEase(TITLE_BTN _btn)
{
	switch (_btn)
	{
	case TitleScene::TITLE_BTN::START_GAME:
		return Easing::EASING_TYPE::ELASTIC_OUT;
		break;
	case TitleScene::TITLE_BTN::TUTORIAL:
		return Easing::EASING_TYPE::BOUNCE;
		break;
	case TitleScene::TITLE_BTN::EXIT:
		return Easing::EASING_TYPE::QUAD_IN_OUT;
		break;
	default:
		break;
	}
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
		imgTitleBack,
		true
	);
	
	//タイトルロゴ
	DrawExtendGraphF(logoPos_.x, logoPos_.y, logoPos_.x + LOGO_SIZE_X, logoPos_.y + LOGO_SIZE_Y, imgTitleLogo, true);


	for (auto& btn : buttons_)
	{
		unsigned int btnCol = UtilityCommon::WHITE;
		if (selectNum_ == static_cast<int>(btn.btnType))
		{
			btnCol = UtilityCommon::RED;
		}
		DrawFormatStringToHandle(
			btn.curPos.x,
			btn.curPos.y,
			btnCol,
			titleFont_,
			btn.btnStr.c_str()
		);
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
			UtilityCommon::BLACK,
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

void TitleScene::UpdateEase(void)
{

	easeDistanceCnt_ += SceneManager::GetInstance().GetDeltaTime();
	logoEaseCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	logoPos_ = easing_->EaseFunc(START_POS, GOAL_POS, (LOGO_EASING_TIME - logoEaseCnt_) / LOGO_EASING_TIME, Easing::EASING_TYPE::ELASTIC_OUT);

	for (auto& btn:buttons_)
	{
		if (btn.isEase)continue;
		if (easeDistanceCnt_ > EASING_DIS_TIME)
		{
			easeDistanceCnt_ = 0.0f;
			btn.isEase = true;
		}
	}
	int i = 0;
	for (auto& btn:buttons_)
	{
		if (!btn.isEase)continue;
		Vector2F goalPos = { BUTTON_START_POS_X,BUTTON_START_POS_Y + BUTTON_DISTANCE * i };
		Easing::EASING_TYPE easeType = DecideEase(btn.btnType);
		
		i++;
		btn.curPos = easing_->EaseFunc(btn.startPos, goalPos, (BUTTON_EASING_TIME - btn.easeCnt) / BUTTON_EASING_TIME, easeType);
		btn.easeCnt -= SceneManager::GetInstance().GetDeltaTime();
	}

	auto itr = std::find_if(buttons_.begin(), buttons_.end(), [this](auto& btn) {return btn.easeCnt > 0.0f; });
	if (itr == buttons_.end() && logoEaseCnt_ < 0.0f)
	{
		//イージングが終わったらメニュー更新へ
		ChangeState(TITLE_STATE::MENU);
	}

}

void TitleScene::UpdateMenu(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	InputManagerS& insS = InputManagerS::GetInstance();

	//selectNum_ = static_cast<int>(selectState_);

	if (insS.IsTrgDown(INPUT_EVENT::UP))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		selectNum_--;
	}
	else if (insS.IsTrgDown(INPUT_EVENT::DOWN))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		selectNum_++;
	}
	//０以下にならないように
	if (selectNum_ < 0) { selectNum_ = static_cast<int>(TITLE_BTN::EXIT); }

	selectNum_ %= static_cast<int>(TITLE_BTN::MAX);
	//selectState_ = static_cast<TITLE_STATE>(selectNum_);
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		if (selectNum_ != static_cast<int>(TITLE_BTN::START_GAME))
		{
			soundMng_.Play(SoundManager::SRC::DESIDE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		}
		else
		{
			soundMng_.Play(SoundManager::SRC::GAME_START_SE, SoundManager::PLAYTYPE::BACK);
		}
		ChangeState(static_cast<TITLE_STATE>(selectNum_));
	}
}

void TitleScene::UpdateScreen(void)
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
	ChangeState(static_cast<TITLE_STATE>(TITLE_STATE::MENU));
}

void TitleScene::UpdateTutorial(void)
{
	//まだ未実装
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
	InputManagerS& insS = InputManagerS::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::LEFT))
	{ 
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		yesNoState_ = YES_NO::YES; 
	}
	else if(insS.IsTrgDown(INPUT_EVENT::RIGHT))
	{ 
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		yesNoState_ = YES_NO::NO; 
	}

	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		if (yesNoState_ == YES_NO::YES) 
		{ 
			Application::GetInstance().IsGameEnd(); 
		}
		else { ChangeState(TITLE_STATE::MENU);}
	}
}

