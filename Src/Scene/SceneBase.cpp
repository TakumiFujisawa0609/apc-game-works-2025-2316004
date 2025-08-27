#include <DxLib.h>
#include <string>
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Utility/UtilityCommon.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) :
	resMng_(ResourceManager::GetInstance())
	,scnMng_(SceneManager::GetInstance())
	,inputMng_(InputManager::GetInstance())
{
	buttnFontHandle_ = -1;
	loadingTime_ = -1;
}

SceneBase::~SceneBase(void)
{
	DeleteFontToHandle(buttnFontHandle_); //フォントの削除
}

void SceneBase::Load(void)
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
	updataFunc_();
	return;
}

void SceneBase::Draw(void)
{
	drawFunc_();
	return;
}

void SceneBase::LoadingUpdate(void)
{
	bool loadTimeOver = UtilityCommon::IsTimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//非同期処理を無効にする
		SetUseASyncLoadFlag(false);

		//初期化処理
		Init();

		//フェードイン開始
		scnMng_.StartFadeIn();

		//通常の処理へ移る
		ChangeNormal();
	}
}

void SceneBase::NormalUpdate(void)
{
}

void SceneBase::LoadingDraw(void)
{
	//NowLoadingの描画
	DrawNowLoading();
}

void SceneBase::NormalDraw(void)
{
}

void SceneBase::ChangeNormal(void)
{
}

void SceneBase::DrawNowLoading(void)
{
	//ロード中
	auto time = scnMng_.GetTotalTime();
	int count = static_cast<int>(time / COMMA_TIME);
	count %= COMMA_MAX_NUM;

	std::wstring loadStr = L"Now loading";
	std::wstring dotStr = L".";

	for (int i = 0; i < count; i++)
	{
		loadStr += dotStr;
	}
	DrawStringToHandle(LOADING_STRING_POS_X, LOADING_STRING_POS_Y, loadStr.c_str(), 0xffffff, buttnFontHandle_);

}
