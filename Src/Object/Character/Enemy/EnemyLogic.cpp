#include "../Manager/Generic/InputManager.h"
#include "../Utility/Utility3D.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "EnemyLogic.h"

EnemyLogic::EnemyLogic(CharacterBase& _playerChara, Transform& _myTrans):
	playerChara_(_playerChara)
	,myTrans_(_myTrans)
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
	//moveDir_ = Utility3D::VECTOR_ZERO;
	VECTOR playerPos = playerChara_.GetTransform().pos;
	VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos,playerPos);
	float dis = Utility3D::Distance(myTrans_.pos, playerPos);
	//moveDir_ = Utility3D::GetMoveVec(myTrans_.pos, playerPos);
	moveDir_ = Utility3D::GetMoveVec(playerPos, myTrans_.pos);
	moveDeg_ = static_cast<float>(Utility3D::AngleDeg(playerPos, myTrans_.pos));
	if (dis > 200.0f)
	{
		moveDir_ = targetVec;
	}





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

