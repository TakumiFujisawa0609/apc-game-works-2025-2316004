#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "MenuController.h"

MenuController::MenuController(void):
	selectMenuNum_(0),
	disSpawnCnt_(0.0f)
{
	easing_ = std::make_unique<Easing>();
}

MenuController::~MenuController(void)
{
}

void MenuController::LoadFont(const std::wstring _fontType, const int _size)
{
	defaultFontHandle_ = _fontType;
	defaultFontSize_ = _size;
	fontHandle_ = CreateFontToHandle(_fontType.c_str(), _size, 0);
}

void MenuController::AddMenu(const int _arrayNum, const std::wstring _menu,const Vector2 _pos)
{
	//情報の代入
	BTN_INFO info;
	info.btnStr = _menu;
	info.startPos = _pos;
	info.curPos = _pos;

	menuList_.emplace(_arrayNum, info);
}

void MenuController::Update(void)
{
}

const int MenuController::GetSizeEasingFontHandle(const int _arrayNum, const int _startSize, const int _goalSize, const float _easeTime, Easing::EASING_TYPE _easeType)
{
	float& easeCnt = menuList_[_arrayNum].easeCnt;
	easeCnt += SceneManager::GetInstance().GetDeltaTime();
	int size = easing_->EaseFunc(_startSize, _goalSize, easeCnt / _easeTime, _easeType);
	if (easeCnt >= _easeTime)
	{
		easeCnt = 0.0f;
	}

	return CreateFontToHandle(defaultFontHandle_.c_str(), size, 0);
}

const bool MenuController::DirectionMenu(const float _disSpawn, const float _easeTime, const int _goalPosX)
{

	//すべてのメニューが演出イージングが終わっているかをチェック
	const bool isAllEaseEnd = std::all_of(menuList_.begin(), menuList_.end(), [](const auto& menu) { return menu.second.isEndDirectEase; });
	if (isAllEaseEnd)
	{
		return true;
	}



	//間隔カウントが一定時間を超えたらイージング開始
	for (auto& menu : menuList_)
	{
		//イージング中またはイージングが終わっているものはスキップ
		if (menu.second.isEase||menu.second.isEndDirectEase)continue;
		if (disSpawnCnt_ > _disSpawn)
		{
			disSpawnCnt_ = 0.0f;
			menu.second.isEase = true;
			break;
		}
	}

	for (auto& menu : menuList_)
	{
		if (menu.second.isEase)
		{
			Vector2 goalPos = { _goalPosX,menu.second.curPos.y };
			menu.second.curPos = easing_->EaseFunc(menu.second.startPos, goalPos, menu.second.easeCnt / _easeTime, Easing::EASING_TYPE::OUT_BACK);
			menu.second.easeCnt += SceneManager::GetInstance().GetDeltaTime();
			if (menu.second.easeCnt >= _easeTime)
			{
				menu.second.isEase = false;
				menu.second.isEndDirectEase = true;
				menu.second.curPos = goalPos;
			}
		}
	}
	if (disSpawnCnt_ > _disSpawn)
	{
		disSpawnCnt_ = 0.0f;
	}
	disSpawnCnt_ += SceneManager::GetInstance().GetDeltaTime();

	return isAllEaseEnd;
}

void MenuController::Draw(void)
{ 
	int dynamicFontHandle = 0;
	unsigned int color = UtilityCommon::WHITE;
	for (auto& menu : menuList_)
	{
		//選択中のメニューはサイズイージングして赤色で描画
		if (menu.first == selectMenuNum_)
		{
			//dynamicFontHandle = GetSizeEasingFontHandle(menu.first, defaultFontSize_, defaultFontSize_ + 10, 0.3f, Easing::EASING_TYPE::COS_BACK);
			dynamicFontHandle = fontHandle_;
			color = UtilityCommon::RED;
		}
		else
		{
			dynamicFontHandle = fontHandle_;
			color = UtilityCommon::WHITE;
		}
		DrawFormatStringToHandle(
			menu.second.curPos.x, menu.second.curPos.y, color, dynamicFontHandle, menu.second.btnStr.c_str());
	}
}

void MenuController::AddSelectMenuNum(void)
{
	selectMenuNum_++;
	if (selectMenuNum_ >= static_cast<int>(menuList_.size()))
	{
		selectMenuNum_ = 0;
	}
}

void MenuController::SubSelectMenuNum(void)
{
	selectMenuNum_--;
	if (selectMenuNum_ < 0)
	{
		selectMenuNum_ = static_cast<int>(menuList_.size()) - 1;
	}
}
