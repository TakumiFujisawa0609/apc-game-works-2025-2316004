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

	//タイトル用フォントの登録
	titleFont_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	menuController_->LoadFont(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE);

	soundMng_.LoadResource(SoundManager::SRC::TITLE_BGM);
	soundMng_.LoadResource(SoundManager::SRC::MOVE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::DESIDE_BTN_SE);
	soundMng_.LoadResource(SoundManager::SRC::GAME_START_SE);

	ButtonUIManager::GetInstance().Load();


	yesNoState_ = YES_NO::NO;
}

void TitleScene::Init(void)
{
	titleSelectFuncTable_ = {
		{TITLE_STATE::EASE_MENU,[this]() {UpdateEase();}},
		{TITLE_STATE::MENU,[this]() {UpdateMenu();}},
		{TITLE_STATE::START_GAME,[this]() {UpdateSelectGame();}},
		//{TITLE_STATE::TUTORIAL,[this]() { UpdateTutorial(); }},
		{TITLE_STATE::SCREEN,[this]() { UpdateScreen(); }},
		{TITLE_STATE::EXIT_MENU,[this]() {UpdateExitMenu();}},
		{TITLE_STATE::SCREEN_MENU,[this]() {UpdateScreenMenu();}},
		{TITLE_STATE::EXIT,[this](){ Application::GetInstance().IsGameEnd(); }}
	};

	buttonStrTable_ = {
		{TITLE_BTN::START_GAME,L"START　GAME"},
		//{TITLE_BTN::TUTORIAL,L"TUTORIAL"},
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
		Vector2 pos = { Application::SCREEN_SIZE_X,static_cast<int>(BUTTON_START_POS_Y)+ BUTTON_DISTANCE*i };
		menuController_->AddMenu(static_cast<int>(button.first), button.second, pos);
		i++;
	}

	SoundManager::GetInstance().Play(SoundManager::SRC::TITLE_BGM,SoundManager::PLAYTYPE::LOOP);
}

Easing::EASING_TYPE TitleScene::DecideEase(TITLE_BTN _btn)
{
	switch (_btn)
	{
	case TitleScene::TITLE_BTN::START_GAME:
		return Easing::EASING_TYPE::ELASTIC_OUT;
		break;
	case TitleScene::TITLE_BTN::SCREEN:
		return Easing::EASING_TYPE::BOUNCE;
		break;
	case TitleScene::TITLE_BTN::EXIT:
		return Easing::EASING_TYPE::QUAD_IN_OUT;
		break;
	default:
		return Easing::EASING_TYPE::CUBIC_OUT;
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
	menuController_->Draw();

	if (selectState_ == TITLE_STATE::EXIT_MENU|| selectState_ == TITLE_STATE::SCREEN)
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

		std::wstring str = L"";
		if (selectState_ == TITLE_STATE::EXIT_MENU)
		{
			str = L"本当にゲームを終了しますか？";
		}
		else if (selectState_ == TITLE_STATE::SCREEN)
		{
			bool isFull = DataBank::GetInstance().GetIsFullScreen();
			if (isFull)
			{
				str = L"通常スクリーンにしますか？";
			}
			else
			{
				str = L"フルスクリーンにしますか？";
			}
		}
		DrawFormatStringToHandle(
			startPos.x + QUESTION_OFFSET,
			startPos.y + QUESTION_OFFSET,
			UtilityCommon::BLACK,
			buttonFontHandle_,
			str.c_str()
		);

		DrawYesNo();
	}


	//決定ボタン
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::B_BUTTON_COL_PUSH, DICITION_BTN_POS, DICITION_BTN_SIZE);
	Vector2F strPos = { DICITION_BTN_POS.x + DICITION_BTN_SIZE / 2.0f,DICITION_BTN_POS.y - BTN_STR_OFFSET_X };
	DrawStringToHandle(
		static_cast<int>(strPos.x),
		static_cast<int>(strPos.y),
		L"決定", 
		UtilityCommon::WHITE, buttonFontHandle_);

	//戻るボタン
	ButtonUIManager::GetInstance().DrawFromCenter(ButtonUIManager::BTN_UI_TYPE::A_BUTTON_COL_PUSH, BACK_BTN_POS, DICITION_BTN_SIZE);
	strPos = { BACK_BTN_POS.x + DICITION_BTN_SIZE / 2.0f,BACK_BTN_POS.y - BTN_STR_OFFSET_X };
	DrawStringToHandle(
		static_cast<int>(strPos.x), 
		static_cast<int>(strPos.y), 
		L"戻る", 
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

	if (menuController_->DirectionMenu(EASING_DIS_TIME, BUTTON_EASING_TIME, BUTTON_START_POS_X))
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

	if (insS.IsTrgDown(INPUT_EVENT::UP)||ins.IsTrgDown(KEY_INPUT_W))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->SubSelectMenuNum();
	}
	else if (insS.IsTrgDown(INPUT_EVENT::DOWN) || ins.IsTrgDown(KEY_INPUT_S))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		menuController_->AddSelectMenuNum();
	}
	selectNum_ = menuController_->GetSelectMenuNum();

	//OKボタンが押されたら
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		SoundManager::SRC se;
		//ゲームスタート以外のボタンなら決定音、ゲームスタートならゲームスタート音を鳴らす
		selectNum_ != static_cast<int>(TITLE_BTN::START_GAME) ? se = SoundManager::SRC::DESIDE_BTN_SE : se = SoundManager::SRC::GAME_START_SE;
		soundMng_.Play(se, SoundManager::PLAYTYPE::BACK);
		ChangeState(static_cast<TITLE_STATE>(selectNum_));
	}
}

void TitleScene::UpdateScreen(void)
{

	UpdateYesNo();
	InputManagerS& insS = InputManagerS::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		if (yesNoState_ == YES_NO::YES)
		{
			ChangeScreenSize();
			ChangeState(static_cast<TITLE_STATE>(TITLE_STATE::MENU));
		}
		else { ChangeState(TITLE_STATE::MENU); }
	}
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
	UpdateYesNo();
	InputManagerS& insS = InputManagerS::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::OK))
	{
		if (yesNoState_ == YES_NO::YES)
		{
			Application::GetInstance().IsGameEnd();
		}
		else { ChangeState(TITLE_STATE::MENU); }
	}
}

void TitleScene::UpdateScreenMenu(void)
{
}

void TitleScene::DrawYesNo(void)
{
	const Vector2 startPos = { Application::SCREEN_HALF_X - (CHECK_EXIT_MENU_SIZE_X / 2)
							,Application::SCREEN_HALF_Y - (CHECK_EXIT_MENU_SIZE_Y / 2) };
	const Vector2 endPos = { Application::SCREEN_HALF_X + (CHECK_EXIT_MENU_SIZE_X / 2),
							Application::SCREEN_HALF_Y + (CHECK_EXIT_MENU_SIZE_Y / 2) };

	int i = 0;
	for (auto& str : yesNoStrTable_)
	{
		unsigned int btnCol = UtilityCommon::WHITE;
		if (yesNoState_ == str.first) { btnCol = UtilityCommon::RED; }
		DrawFormatStringToHandle(
			startPos.x + YES_NO_DISTANCE_X + i * YES_NO_DISTANCE_Y,
			startPos.y + YES_NO_DISTANCE_Y,
			btnCol,
			titleFont_,
			str.second.c_str()
		);
		i++;
	}
}

void TitleScene::UpdateYesNo(void)
{
	InputManagerS& insS = InputManagerS::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	if (insS.IsTrgDown(INPUT_EVENT::LEFT) || ins.IsTrgDown(KEY_INPUT_A))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		yesNoState_ = YES_NO::YES;
	}
	else if (insS.IsTrgDown(INPUT_EVENT::RIGHT) || ins.IsTrgDown(KEY_INPUT_D))
	{
		soundMng_.Play(SoundManager::SRC::MOVE_BTN_SE, SoundManager::PLAYTYPE::BACK);
		yesNoState_ = YES_NO::NO;
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

