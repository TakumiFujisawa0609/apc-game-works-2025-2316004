#include"../Player/ActionController.h"
#include"../../Common/AnimationController.h"
#include"../Base/CharacterBase.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
#include "Idle.h"

Idle::Idle(ActionController& _actCntl):
	ActionBase(_actCntl)
{
}

Idle::~Idle(void)
{
}

void Idle::Init(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::IDLE));
}

void Idle::Update(void)
{
	//移動
	if (!Utility3D::EqualsVZero(actionCntl_.GetInput().GetDir()))
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::MOVE);
		return;
	}
	//カード使用
	if (/*actionCntl_.IsCardDisitionControll()&&*/actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::CARD_ACTION);
		return;
	}

	if (actionCntl_.GetInput().GetIsAct().isJump)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::JUMP);
		return;
	}


	//何もしないが、待機モーションとかずっと止まってるときにアニメーションを変えたりしたい
}