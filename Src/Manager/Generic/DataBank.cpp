#include "InputManager.h"
#include "DataBank.h"

DataBank* DataBank::instance_ = nullptr;

void DataBank::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DataBank();
	}
	instance_->Init();
}

DataBank& DataBank::GetInstance(void)
{
    return *instance_;
}

void DataBank::Init(void)
{
	playerNum_ = 0;
	maxPlayerNum_ = GetJoypadNum();
	isFullScreen_ = false;
}

void DataBank::Destroy(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

bool DataBank::SetPlayerNum(int _num)
{
	//maxPlayerNum_ = GetJoypadNum();
	//if (num <= maxPlayerNum_)
	//{
		playerNum_ = _num;
		return true;
	//}
	return false;
}

int DataBank::GetPlayerNum(void)
{
	return playerNum_;
}

void DataBank::SetIsFullScreen(const bool _isFullScreen)
{
	isFullScreen_ = _isFullScreen;
	ChangeWindowMode(!isFullScreen_);
}


DataBank::DataBank(void)
{
	maxPlayerNum_ = 0;
	playerNum_ = 0;
}

DataBank::~DataBank(void)
{
}
