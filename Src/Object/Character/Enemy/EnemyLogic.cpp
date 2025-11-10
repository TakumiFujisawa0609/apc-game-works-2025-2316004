#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Utility/Utility3D.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "EnemyLogic.h"

EnemyLogic::EnemyLogic(Transform& _myTrans,CharacterBase& _playerChara):
	myTrans_(_myTrans),
	playerChara_(_playerChara),
	scnMng_(SceneManager::GetInstance())
{
}

EnemyLogic::~EnemyLogic(void)
{
}

void EnemyLogic::Init(void)
{
	cardCoolCnt_ = CARD_COOL_TIME;
	attackType_ = ENEMY_ATTACK_TYPE::NONE;
	moveCnt_ = -1.0f;

	freezeCnt_ = 1.5f;
}

void EnemyLogic::Update(void)
{
	//すべての行動フラグをリセット
	isAct_ = {};
	moveDir_ = Utility3D::VECTOR_ZERO;
	VECTOR playerPos = playerChara_.GetTransform().pos;
	VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos,playerPos);


	if(freezeCnt_ > 0.0f)
	{
		freezeCnt_ -= scnMng_.GetDeltaTime();
		return;
	}

	//y座標までは追従しない
	targetVec.y = 0.0f;
	distance_ = static_cast<float>(Utility3D::Distance(myTrans_.pos, playerPos));

	cardCoolCnt_ -= scnMng_.GetDeltaTime();

	moveCnt_ -= scnMng_.GetDeltaTime();

	if (distance_ > 250.0f&&!isActioning_)
	{
		//距離が遠いときは近づく
		isAct_.isRun = true;
		moveDir_ = targetVec;
		moveDeg_ = static_cast<float>(Utility3D::AngleDeg(playerPos, myTrans_.pos));
		return;
	}

	//硬直がなかったらカード使用可能
	if (freezeCnt_ <= 0.0f)
	{
		DesideAction();
		isAct_.isCardUse = true;
		cardCoolCnt_ = CARD_COOL_TIME;
	}


#ifdef _DEBUG
	DebugUpdate();
#endif // _DEBUG

}

const VECTOR EnemyLogic::GetLookAtTargetDir(void)const
{
	const VECTOR& playerPos = playerChara_.GetTransform().pos;
	const VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos, playerPos);
	return targetVec;
}

const float EnemyLogic::GetLookAtTargetDeg(void) const
{
	float deg = static_cast<float>(Utility3D::AngleDeg(playerChara_.GetTransform().pos, myTrans_.pos));
	return deg;
}

void EnemyLogic::DebugUpdate(void)
{
	//移動角度を決める
	if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility3D::DIR_F;
	}
	else if (InputManager::GetInstance().IsNew(KEY_INPUT_LEFT))
	{
		moveDeg_ = LEFT_DEG;
		moveDir_ = Utility3D::DIR_L;
	}
	else if (InputManager::GetInstance().IsNew(KEY_INPUT_DOWN))
	{
		moveDeg_ = BACK_DEG;
		moveDir_ = Utility3D::DIR_B;
	}
	else if (InputManager::GetInstance().IsNew(KEY_INPUT_RIGHT))
	{
		moveDeg_ = RIGHT_DEG;
		moveDir_ = Utility3D::DIR_R;
	}

	
	////カード移動＆使用
	//if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_LEFT))
	//{
	//	isAct_.isCardMoveLeft = true;
	//}
	//else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RIGHT))
	//{
	//	isAct_.isCardMoveRight = true;
	//}
	//else if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RCONTROL))
	//{
	//	isAct_.isCardUse = true;
	//}
}

void EnemyLogic::DebugDraw(void)
{
	DrawFormatString(50, 100, 0x000000,L"Dis(%f)",distance_);
}

void EnemyLogic::DesideAction(void)
{
	VECTOR playerPos = playerChara_.GetTransform().pos;
	VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos, playerPos);

	//ランダムの数値取得
	int rand = GetRand(100);

	
	if (distance_ > 200.0f)
	{
		//遠距離時
		if (rand < weight_.normal)
		{
			//通常攻撃
			attackType_ = ENEMY_ATTACK_TYPE::NORMAL;
		}
		else if(rand < weight_.jump)
		{
			//ジャンプ
			attackType_ = ENEMY_ATTACK_TYPE::JUMP;
		}
		else
		{
			//転がる攻撃
			attackType_ = ENEMY_ATTACK_TYPE::ROLE;
		}
	}
	else
	{
		if (rand < weight_.normal)
		{
			//通常攻撃
			attackType_ = ENEMY_ATTACK_TYPE::NORMAL;
		}
		else if (rand < weight_.jump)
		{
			//ジャンプ攻撃
			attackType_ = ENEMY_ATTACK_TYPE::JUMP;
		}
		else
		{
			//咆哮
			attackType_ = ENEMY_ATTACK_TYPE::ROAR;
		}
	}

	//前回の攻撃を代入する
	prevAttackType_ = attackType_;

	//SetFreezeCntByAttackType(attackType_);
	isActioning_ = true;
}

void EnemyLogic::SetFreezeCntByAttackType(void)
{
	switch (prevAttackType_)
	{
	case LogicBase::ENEMY_ATTACK_TYPE::NONE:
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::NORMAL:
		freezeCnt_ = NORMAL_FREEZE_TIME;
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::JUMP:
		freezeCnt_ = JUMP_FREEZE_TIME;
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::ROAR:
		freezeCnt_ = ROAR_FREEZE_TIME;
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::ROLE:
		freezeCnt_ = ROLE_FREEZE_TIME;
		break;
	default:
		break;
	}
}

