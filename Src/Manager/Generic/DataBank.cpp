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

DataBank::DataBank(void):
	maxPlayerNum_(),
	playerNum_(),
	isFullScreen_(false)
{

}

DataBank::~DataBank(void)
{
}
