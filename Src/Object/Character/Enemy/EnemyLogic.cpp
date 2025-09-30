#include"../Manager/Generic/InputManager.h"
#include "EnemyLogic.h"

EnemyLogic::EnemyLogic(void)
{
}

EnemyLogic::~EnemyLogic(void)
{
}

void EnemyLogic::Init(void)
{
	// Initialization code for EnemyLogic can be added here
}

void EnemyLogic::Update(void)
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

