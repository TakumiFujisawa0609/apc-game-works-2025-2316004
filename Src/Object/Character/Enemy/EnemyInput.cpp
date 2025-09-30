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
	//すべての行動フラグをリセット
	isAct_ = {};
	//デバッグ用の入力処理
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_LEFT))
	{
		isAct_.isCardMoveLeft = true;
	}
	else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RIGHT))
	{
		isAct_.isCardMoveRight = true;
	}
	else if(InputManager::GetInstance().IsTrgDown(KEY_INPUT_RCONTROL))
	{
		isAct_.isCardUse = true;
	}
}

