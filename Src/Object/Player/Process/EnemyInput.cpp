#include"../Manager/Generic/InputManager.h"
#include "EnemyInput.h"

EnemyInput::EnemyInput(void)
{
}

EnemyInput::~EnemyInput(void)
{
}

void EnemyInput::Init(void)
{
	// Initialization code for EnemyInput can be added here
}

void EnemyInput::Update(void)
{
	actCntl_ = ACT_CNTL::NONE; //初期状態は何も入力されてない
	//デバッグ用の入力処理
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_LEFT))
	{
		actCntl_ = ACT_CNTL::CARD_MOVE_LEFT;
	}
	else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RIGHT))
	{
		actCntl_ = ACT_CNTL::CARD_MOVE_RIGHT;
	}
	else if(InputManager::GetInstance().IsTrgDown(KEY_INPUT_LCONTROL))
	{
		actCntl_ = ACT_CNTL::_CARD_USE;
	}


}

