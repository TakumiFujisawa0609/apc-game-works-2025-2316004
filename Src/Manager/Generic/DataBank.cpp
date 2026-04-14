#include "InputManager.h"
#include "DataBank.h"


void DataBank::Init(void)
{
	playerNum_ = 0;
	maxPlayerNum_ = GetJoypadNum();
	isFullScreen_ = false;
}

void DataBank::SetIsFullScreen(const bool _isFullScreen)
{
	isFullScreen_ = _isFullScreen;
    ChangeWindowMode(!isFullScreen_);
}

void DataBank::ReleaseCardData(void)
{
	characterCardDecks_.clear();
}

void DataBank::AddCardData(const CHARACTER_TYPE _charaType, CardBase::CARD_STATUS _status)
{
	characterCardDecks_[_charaType].emplace_back(_status);
}

std::vector<CardBase::CARD_STATUS> DataBank::GetCardDatas(const CHARACTER_TYPE _charaType)
{
	return characterCardDecks_[_charaType];
}

DataBank::DataBank(void):
	maxPlayerNum_(),
	playerNum_(),
	isFullScreen_(false)
{

}

DataBank::~DataBank(void)
{
}
