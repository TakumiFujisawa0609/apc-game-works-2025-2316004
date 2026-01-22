#include "PauseScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Utility/UtilityCommon.h"

PauseScene::PauseScene(void)
{
	//更新関数のセット
	updateFunc_ = std::bind(&PauseScene::NormalUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&PauseScene::NormalDraw, this);

	imgTest_ = -1;
	pauseFont_ = -1;
	selectIndex_ = 0;
	
	//リストごとに処理を分ける
	listFuncTable_ =
	{
		{LIST::RESUME,[this]()
		{
			//ポーズを解除して前のシーンに戻る
			scnMng_.PopScene();
		}},
		{LIST::TITLE,[this]()
		{
			//タイトルシーンに戻る
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}}
	};
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Load(void)
{
	imgTest_ = resMng_.Load(ResourceManager::SRC::TEST).handleId_;
	pauseFont_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, FONT_THICK);
}

void PauseScene::Init(void)
{	
	
}

void PauseScene::NormalUpdate(void)
{
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		//シーンを戻す
		scnMng_.PopScene();
		return;
	}
	else if (inputMng_.IsTrgDown(KEY_INPUT_DOWN))
	{
		selectIndex_ = (selectIndex_ - 1 + LIST_MAX) % LIST_MAX;
	}
	else if (inputMng_.IsTrgDown(KEY_INPUT_UP))
	{
		selectIndex_ = (selectIndex_ + 1) % LIST_MAX;
	}
	else if (inputMng_.IsTrgDown(KEY_INPUT_RETURN))
	{
		listFuncTable_[static_cast<LIST>(selectIndex_)]();
		return;
	}
}

void PauseScene::NormalDraw(void)
{
	static constexpr int MARGINT = 50;
	static constexpr int OFFSET_Y = 200;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PAUSE_ALPHA);
	DrawBox(
		MARGINT,
		MARGINT,
		Application::SCREEN_SIZE_X- MARGINT,
		Application::SCREEN_SIZE_Y- MARGINT,
		UtilityCommon::WHITE,
		true);	
	
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		1.0f,
		0.0f,
		imgTest_,
		true,
		false
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < LIST_MAX; ++i)
	{
		//カラーを設定
		int color = UtilityCommon::BLUE;
		if (i == selectIndex_)
		{
			color = UtilityCommon::RED; // 選択中は赤色
		}

		//座標位置を設定
		int posX = static_cast<int>(Application::SCREEN_HALF_X - pasueList_[i].length() * FONT_SIZE / 2);
		int posY = Application::SCREEN_HALF_Y - OFFSET_Y * i;

		//文字列を描画
		DrawFormatStringToHandle(
			posX,
			posY,
			color,
			pauseFont_,
			pasueList_[i].c_str());
	}
}