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
	distance_ = Utility3D::Distance(myTrans_.pos, playerPos);
	//moveDir_ = Utility3D::GetMoveVec(myTrans_.pos, playerPos);

	moveDeg_ = static_cast<float>(Utility3D::AngleDeg(playerPos, myTrans_.pos));

	cardCoolCnt_ -= scnMng_.GetDeltaTime();
	moveCnt_ -= scnMng_.GetDeltaTime();


	if (cardCoolCnt_ < 0.0f)
	{
		DesideAction();
		isAct_.isCardUse = true;
		cardCoolCnt_ = CARD_COOL_TIME;
	}

	//if (dis > ATK_RANGE&&moveCnt_<0.0f)
	//{
	//	moveDir_ = targetVec;
	//}
	//else if(dis <= ATK_RANGE)
	//{
	//	moveCnt_ = MOVE_COOL_TIME;
	//}


#ifdef _DEBUG
	DebugUpdate();
#endif // _DEBUG

}

void EnemyLogic::DebugUpdate(void)
{
	//デバッグ用の入力処理
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_LEFT))
	{
		isAct_.isCardMoveLeft = true;
	}
	else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RIGHT))
	{
		isAct_.isCardMoveRight = true;
	}
	else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RCONTROL))
	{
		isAct_.isCardUse = true;
	}
}

void EnemyLogic::DesideAction(void)
{
	VECTOR playerPos = playerChara_.GetTransform().pos;
	VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos, playerPos);

	int rand = GetRand(100);
	if (distance_ > 1000.0f)
	{
		moveDir_ = targetVec;
	}
	else if (distance_ > 300.0f)
	{
		if (rand < weight_.normal)
		{
			//通常攻撃
		}
		else
		{
			//ジャンプ
		}
	}
}

