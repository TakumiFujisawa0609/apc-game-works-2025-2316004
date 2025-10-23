#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Utility/Utility3D.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "EnemyLogic.h"

EnemyLogic::EnemyLogic(CharacterBase& _playerChara, Transform& _myTrans):
	playerChara_(_playerChara)
	,scnMng_(SceneManager::GetInstance())
	,myTrans_(_myTrans)
{
}

EnemyLogic::~EnemyLogic(void)
{
}

void EnemyLogic::Init(void)
{
	cardCoolCnt_ = CARD_COOL_TIME;
	moveCnt_ = -1.0f;
}

void EnemyLogic::Update(void)
{
	//すべての行動フラグをリセット
	isAct_ = {};
	moveDir_ = Utility3D::VECTOR_ZERO;
	VECTOR playerPos = playerChara_.GetTransform().pos;
	VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos,playerPos);
	float dis = Utility3D::Distance(myTrans_.pos, playerPos);
	//moveDir_ = Utility3D::GetMoveVec(myTrans_.pos, playerPos);

	moveDeg_ = static_cast<float>(Utility3D::AngleDeg(playerPos, myTrans_.pos));

	cardCoolCnt_ -= scnMng_.GetDeltaTime();
	moveCnt_ -= scnMng_.GetDeltaTime();

	if (dis > ATK_RANGE&&moveCnt_<0.0f)
	{
		moveDir_ = targetVec;
	}
	else if(dis <= ATK_RANGE)
	{
		moveCnt_ = MOVE_COOL_TIME;
	}
	if (cardCoolCnt_ < 0.0f)
	{
		isAct_.isCardUse = true;
		cardCoolCnt_ = CARD_COOL_TIME;
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

